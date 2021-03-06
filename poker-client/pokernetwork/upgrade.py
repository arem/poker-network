#
# Copyright (C) 2008 Loic Dachary <loic@dachary.org>
# Copyright (C) 2005, 2006 Mekensleep <licensing@mekensleep.com>
#                          24 rue vieille du temple, 75004 Paris
#
# This software's license gives you freedom; you can copy, convey,
# propagate, redistribute and/or modify this program under the terms of
# the GNU Affero General Public License (AGPL) as published by the Free
# Software Foundation (FSF), either version 3 of the License, or (at your
# option) any later version of the AGPL published by the FSF.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero
# General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program in a file in the toplevel directory called
# "AGPLv3".  If not, see <http://www.gnu.org/licenses/>.
#
# Authors:
#  Loic Dachary <loic@dachary.org>
#

import os
import platform

if platform.system() == "Windows":
    import win32api
    import win32con

from re import match

from pokernetwork import dispatch

from pokernetwork.pokerchildren import PokerRsync, RSYNC_DONE, RSYNC_FAILED, RSYNC_NO_NETWORK, RSYNC_HOST_DOES_NOT_RESPOND

class Constants:
    EXCLUDES = []
    UPGRADES_DIR = None
    BANDWIDTH = [ ] # [ "--bwlimit=128" ]
    # 2005/10 : no -z because not supported on all platforms
    RSYNC = [ "rsync", "--timeout=60" ]
    
FAILED = "//event/pokernetwork/upgrade/failed"
TICK = "//event/pokernetwork/upgrade/tick"
NEED_UPGRADE = "//event/pokernetwork/upgrade/need_upgrade"
CLIENT_VERSION_OK = "//event/pokernetwork/upgrade/client_version_ok"
UPGRADE_READY = "//event/pokernetwork/upgrade/upgrade_ready"
NO_NETWORK = "//event/pokernetwork/upgrade/no_network"
HOST_DOES_NOT_RESPOND = "//event/pokernetwork/upgrade/host_does_not_respond"

class CheckClientVersion(PokerRsync):

    def __init__(self, config, settings, version, callback):
        if match(".*::", settings.headerGet("/settings/rsync/@source")):
            PokerRsync.__init__(self, config, settings, Constants.RSYNC + [ "@SOURCE@/" ])
        else:
            PokerRsync.__init__(self, config, settings, Constants.RSYNC + [ '--dry-run', '--list-only', "@SOURCE@/", "/most/unlikely/directory" ])
        self.version_compare = "%03d%03d%03d" % version
        self.version = version
        if self.verbose > 1:
            self.message("CheckClientVersion checking version %s against server" % str(self.version))
        self.callback = callback
        self.spawn()
        self.need_upgrade = False
        self.version_matched = False

    def line(self, line):
        if self.verbose > 2: self.message(line)
        result = match(".*(\d+).(\d+).(\d+)$", line)
        if result:
	    self.version_matched = True
            result = tuple(map(int, result.groups()))
            if self.verbose > 2:
                self.message("compare %s against %s" % ( str(result), str(self.version)))
            version = "%03d%03d%03d" % result
            if version > self.version_compare:
		self.version_compare = version
                self.version = result
                self.need_upgrade = True

    def done(self):
        self.callback(self.need_upgrade, "%d.%d.%d" % self.version)

    def failed(self, logs, reason):
        self.error("*CRITICAL* CheckClientVersion.failed logs:%s reason:%s" % (logs, reason))
        if self.version_matched:
	    self.done()
        else:
	    self.publishEvent(RSYNC_FAILED, logs, reason)

DRY_RUN_DONE = "//event/pokernetwork/upgrade/dry_run_done"

class DryrunUpgrade(PokerRsync):

    def __init__(self, config, settings, version):
        PokerRsync.__init__(self, config, settings, Constants.RSYNC + Constants.EXCLUDES + [ "--dry-run", "-av", "--delete", "--progress", "--log-format=FILE:%f", "@SOURCE@/" + version + "/", "@TARGET@" ])
        self.files_count = 0
        self.files_total = 0.0

    def spawn(self):
        self.publishEvent(TICK, 0.0, "Looking for the new client upgrade")
        PokerRsync.spawn(self)
        
    def line(self, line):
        if self.verbose > 2: self.message(line)
        if match("^FILE:", line):
            self.files_count += 1
            if self.files_total > 0.0:
                self.publishEvent(TICK, self.files_count / self.files_total, None)
        else:
            result = match(".*?(\d+)\s+files\s+to\s+consider", line)
            if result:
                self.files_total = float(result.group(1))

    def done(self):
        self.publishEvent(TICK, 1.0, None)
        self.publishEvent(DRY_RUN_DONE, float(self.files_count))

GET_PATCH_DONE = "//event/pokernetwork/upgrade/get_patch_done"

class GetPatch(PokerRsync):

    def __init__(self, config, settings, version, files_total):
        PokerRsync.__init__(self, config, settings, Constants.RSYNC + Constants.EXCLUDES + Constants.BANDWIDTH + [ "--only-write-batch=%s/patch" % Constants.UPGRADES_DIR, "--delete", "-a", "--log-format=FILE:%f", "@SOURCE@/" + version + "/", "@TARGET@" ])
        self.files_count = 0
        self.files_total = files_total

    def spawn(self):
        self.publishEvent(TICK, 0.0, "Retrieving the client upgrade")
        PokerRsync.spawn(self)
        
    def line(self, line):
        if self.verbose > 2: self.message(line)
        if match("^FILE:", line):
            self.files_count += 1
            if self.files_total > 0.0:
                self.publishEvent(TICK, self.files_count / self.files_total, None)

    def done(self):
        self.publishEvent(TICK, 1.0, None)
        self.publishEvent(GET_PATCH_DONE)

class Upgrader(dispatch.EventDispatcher):

    def __init__(self, config, settings):
        self.verbose = settings.headerGetInt("/settings/@verbose")
        dispatch.EventDispatcher.__init__(self)
        self.config = config
        self.settings = settings
        self.target = self.settings.headerGet("/settings/rsync/@target")
        if platform.system() == "Windows":
            try:
                reg_key = win32api.RegOpenKey(win32con.HKEY_CURRENT_USER, "Software\Mekensleep\Pok3d")
            except:
                reg_key = win32api.RegOpenKey(win32con.HKEY_LOCAL_MACHINE, "Software\Mekensleep\Pok3d") 
            self.target = win32api.RegQueryValue(reg_key, "")
            win32api.RegCloseKey(reg_key)

#        self.settings.headerSet("/settings/rsync/@target", self.target)

        self.upgrades = self.settings.headerGet("/settings/rsync/@upgrades")
        Constants.UPGRADES_DIR = self.target + "/" + self.upgrades
        source = self.settings.headerGet("/settings/rsync/@source")

    def error(self, string):
        self.message("ERROR " + str(string))

    def message(self, string):
        print "Upgrade: " + str(string)

    def failed(self, logs, reason):
        self.publishEvent(FAILED, logs, reason)

    def checkClientVersionFailedNoNetwork(self, logs, reason):
        self.publishEvent(NO_NETWORK,logs, reason)
        self.error("*CRITICAL* checkClientVersionFailedNoNetwork logs:%s reason:%s" % (logs, reason))
        
    def checkClientVersionFailedHostDoesNotRespond(self, logs, reason):
        self.publishEvent(HOST_DOES_NOT_RESPOND,logs, reason)
        self.error("*CRITICAL* checkClientVersionFailedHostDoesNotRespond logs:%s reason:%s" % (logs, reason))
        
    def checkClientVersion(self, version):
        if self.verbose > 1: self.message("Upgrade::checkClientVersion(" + str(version) + ")" )
        self.publishEvent(TICK, 0.0, "Checking for new client version")
        checker = CheckClientVersion(self.config, self.settings, version, self.checkClientVersionDone)
        checker.registerHandler(RSYNC_NO_NETWORK, self.checkClientVersionFailedNoNetwork)
        checker.registerHandler(RSYNC_HOST_DOES_NOT_RESPOND, self.checkClientVersionFailedHostDoesNotRespond)
        checker.registerHandler(RSYNC_FAILED, self.failed)

    def checkClientVersionDone(self, need_upgrade, version):
        if need_upgrade:
            self.publishEvent(TICK, 1.0, "A new version is available")
            self.publishEvent(NEED_UPGRADE, version)
        else:
            self.publishEvent(TICK, 1.0, "Ok")
            self.publishEvent(CLIENT_VERSION_OK)

    def getUpgrade(self, version, excludes):
        if self.verbose >= 1: self.message("Upgrader::getUpgrade")
        if platform.system() == "Windows":
            upgrades = "/upgrades"
        else:
            upgrades = self.upgrades
        Constants.EXCLUDES = map(lambda pattern: "--exclude=" + pattern, (upgrades, "poker.client.xml", "Pok3d.lnk") + excludes)        
        self.upgradeStage1(version)

    def upgradeStage1(self, version):
        if self.verbose > 1: self.message("Upgrade::upgrade to version " + version)
        stage1 = DryrunUpgrade(self.config, self.settings, version)
        stage1.registerHandler(TICK, lambda ratio, message: self.publishEvent(TICK, ratio, message))
        stage1.registerHandler(DRY_RUN_DONE, lambda files_count: self.upgradeStage2(version, files_count))
        stage1.registerHandler(RSYNC_FAILED, self.failed)
        stage1.spawn()

    def upgradeStage2(self, version, files_count):
        self.publishEvent(TICK, 0.0, "Upgrading the upgrade system")
        rsync = PokerRsync(self.config, self.settings, Constants.RSYNC + Constants.EXCLUDES + Constants.BANDWIDTH + [ "--delete", "-a", "@SOURCE@/" + "%s/%s/" % ( version, self.upgrades ), Constants.UPGRADES_DIR ])
        rsync.registerHandler(RSYNC_DONE, lambda: self.upgradeStage3(version, files_count))
        rsync.registerHandler(RSYNC_FAILED, self.failed)
        rsync.spawn()

    def upgradeStage3(self, version, files_count):
        self.publishEvent(TICK, 1.0, "Upgrade system upgraded")
        stage2 = GetPatch(self.config, self.settings, version, files_count)
        stage2.registerHandler(TICK, lambda ratio, message: self.publishEvent(TICK, ratio, message))
        stage2.registerHandler(GET_PATCH_DONE, self.upgradeReady)
        stage2.registerHandler(RSYNC_FAILED, self.failed)
        stage2.spawn()
        
    def upgradeReady(self):
        if self.verbose > 1: self.message("Upgrade::upgradeReady")
        self.publishEvent(UPGRADE_READY, self.target, Constants.UPGRADES_DIR)
