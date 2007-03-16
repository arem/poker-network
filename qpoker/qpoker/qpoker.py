#!/usr/bin/python
#
# Copyright (C) 2007 Mekensleep
#
# Mekensleep
# 24 rue vieille du temple
# 75004 Paris
#       licensing@mekensleep.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA.
#
# Authors:
#  Johan Euphrosine <proppy@aminche.com>
#

import sys
import string
import qt4reactor
qt4reactor.install()
from twisted.internet import reactor
from pokernetwork.pokerclient import PokerClientFactory
from pokernetwork.pokernetworkconfig import Config
from pokerui.pokerdisplay import PokerDisplay
from pokerui.pokerrenderer import PokerRenderer
from pokerui.pokerinterface import PokerInterface
from pokernetwork.pokerpackets import PACKET_POKER_CHAT, PACKET_POKER_BOARD_CARDS, PACKET_POKER_START, PACKET_POKER_PLAYER_ARRIVE, PACKET_POKER_PLAYER_LEAVE, PACKET_POKER_PLAYER_CHIPS, PACKET_POKER_POSITION, PacketPokerSeat, PacketPokerFold, PacketPokerCheck, PacketPokerCall, PacketPokerRaise
from pokernetwork.pokerclientpackets import PACKET_POKER_POT_CHIPS, PACKET_POKER_CHIPS_POT_RESET
from pokerengine.pokerchips import PokerChips
from qpokerwidget import QPokerWidget
from PyQt4.QtGui import QApplication

class DummyPokerDisplay(PokerDisplay):
    def __init__(self, *args, **kwargs):
        PokerDisplay.__init__(self, *args, **kwargs)
        self.widget = QPokerWidget()
        self.widget.seatClicked = lambda seat: self.seatClicked(seat)
        self.widget.foldClicked = lambda: self.foldClicked()
        self.widget.checkClicked = lambda: self.checkClicked()
        self.widget.callClicked = lambda: self.callClicked()
        self.widget.betClicked = lambda: self.betClicked()
        self.widget.show()
        self.serial2seat = {}
    def seatClicked(self, seat):
        protocol = self.protocol
        self.renderer.getSeat(PacketPokerSeat(game_id = protocol.getCurrentGameId(),
                                              serial = protocol.getSerial(),
                                              seat = seat))
    def foldClicked(self):
        protocol = self.protocol
        self.renderer.interactorSelected(PacketPokerFold(game_id = protocol.getCurrentGameId(), serial = protocol.getSerial()))            
    def checkClicked(self):
        protocol = self.protocol
        self.renderer.interactorSelected(PacketPokerCheck(game_id = protocol.getCurrentGameId(), serial = protocol.getSerial()))
    def callClicked(self):
        protocol = self.protocol
        self.renderer.interactorSelected(PacketPokerCall(game_id = protocol.getCurrentGameId(), serial = protocol.getSerial()))
    def betClicked(self):
        protocol = self.protocol
        self.renderer.interactorSelected(PacketPokerRaise(game_id = protocol.getCurrentGameId(),serial = protocol.getSerial(), amount = 0))
    def render(self, packet):
        print "PokerDisplay2D::render: " + str(packet)
        if packet.type == PACKET_POKER_CHAT:
            message = packet.message
            message = message.replace('\n', '')
            self.widget.renderChat(message)
        elif packet.type == PACKET_POKER_BOARD_CARDS:
            game = self.factory.getGame(packet.game_id)
            board = game.eval.card2string(packet.cards)
            self.widget.renderBoard(board)
        elif packet.type == PACKET_POKER_START:
            self.widget.renderStart()
        elif packet.type == PACKET_POKER_PLAYER_ARRIVE:
            self.serial2seat[packet.serial] = packet.seat
            self.widget.renderPlayerArrive(packet.seat, packet.name)
        elif packet.type == PACKET_POKER_PLAYER_LEAVE:
            del self.serial2seat[packet.serial]
            self.widget.renderPlayerLeave(packet.seat)
        elif packet.type == PACKET_POKER_PLAYER_CHIPS:
            self.widget.renderPlayerChips(self.serial2seat[packet.serial], packet.money, packet.bet)
        elif packet.type == PACKET_POKER_POT_CHIPS:
            game = self.factory.getGame(packet.game_id)
            pots = game.getPots()
            self.widget.renderPot(packet.index, pots['pots'][packet.index][0])
        elif packet.type == PACKET_POKER_CHIPS_POT_RESET:
            self.widget.renderPotReset()
        elif packet.type == PACKET_POKER_POSITION:
            if packet.serial == 0:
                self.widget.renderPositionReset()
            else:
                self.widget.renderPosition(self.serial2seat[packet.serial])

class DummyPokerRenderer(PokerRenderer):
    def __init__(self, *args, **kwargs):
        PokerRenderer.__init__(self, *args, **kwargs)
class DummyPokerInterface(PokerInterface):
    def __init__(self, *args, **kwargs):
        self.verbose = 5
        PokerInterface.__init__(self, *args, **kwargs)
    def command(self, *args):
        print "DummyPokerInterface.command %s" % str(args)
        if args[0] == "login":
            #call later to allow callback to be attached
            reactor.callLater(0, lambda: self.event("login", "ok", "proppy", "pqsspqss", "1"))
        elif args[0] == "menu":
            pass
        elif args[0] == "chat":
            pass
        elif args[0] == "buy_in":
            if args[1] == 'params':
                (min, max) = args[2:4]
                reactor.callLater(0, lambda: self.event("buy_in", max))
        elif args[0] == "sit_actions":
            pass
        elif args[0] == "cashier":
            pass
        elif args[0] == "tournaments":
            pass
        elif args[0] == "lobby":
            if args[1] == "info":
                reactor.callLater(0, lambda: self.event("lobby", "join", "100"))
        elif args[0] == "check_warning":
            reactor.callLater(0, lambda: self.event("check_warning", "fold"))
        else:
            print "*ERROR* command not implemented"
            raise UserWarning
        
class DummyPokerClientFactory(PokerClientFactory):
    def __init__(self, settings, config):
        self.verbose = 5
        PokerClientFactory.__init__(self, settings = settings, config = config)
        self.display = DummyPokerDisplay(settings = settings,
                                         config = config,
                                         factory = self)
        self.renderer = DummyPokerRenderer(self)
        self.interface = DummyPokerInterface()
        self.renderer.interfaceReady(self.interface)
    def buildProtocol(self, addr):
        protocol = PokerClientFactory.buildProtocol(self, addr)
        self.display.setProtocol(protocol)
        self.renderer.setProtocol(protocol)
        return protocol

def run():
    settings = Config([''])
    settings.load('qpoker.xml')
    config = Config([''])
    config.load('client.xml')
    (host, port) = settings.headerGet("/settings/servers").split(" ")[0].split(":")
    client = DummyPokerClientFactory(settings, config)
    reactor.connectTCP(host, int(port), client)
    reactor.run()
    
if __name__ == '__main__':
    run()
