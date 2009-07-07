main: mov zip;

top:; @date

host := $(shell hostname -s)
self := $(firstword $(MAKEFILE_LIST))
base := $(basename $(notdir $(self)))

$(self):;

RUN := cat
RUN := dash

p = poker-network

mov:
	cd /var/log; \
		ls $p-server*.log.* | xargs stat -c 'echo $$(date +%%F -d "%y") %n' | sh \
		| sort | sed -re "s/(.*) (.*)/mkdir -p \1-$p; mv \2 \1-$p/" | $(RUN)

zip:; cd /var/log; find 20??-??-??-poker-network -type f | grep -Fv .bz2 | xargs bzip2 -v

bin := /usr/local/bin

user := root
group := root
install.cmd = install --backup=numbered -o $(user) -g $(group) -m $1 $< $@
cron.d := /etc/cron.d
$(bin)/$(self): $(self); $(call install.cmd,555)
#$(cron.d)/$(base): $(cron); $(call install.cmd,444)
install: $(bin)/$(self)
