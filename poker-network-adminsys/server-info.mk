HOST := $(shell hostname -s)
DATABASE := pythonpokernetwork
CONFDIR := /etc/poker-network

self := $(firstword $(MAKEFILE_LIST))
base := $(basename $(notdir $(self)))

$(self):;

mysql := mysql -u root --silent $(DATABASE)

serial.sql := "select serial from resthost where host like '$(HOST)%'"
serial := $(shell echo $(serial.sql) | $(mysql))

players.sql = \
	'select count(*) from user2table,route where route.table_serial = user2table.table_serial and route.resthost_serial = $1'

ports.sql = 'select port from resthost where serial = $1'

tables.sql = 'select * from route where resthost_serial = $1'

serials:; @echo $(serial.sql) | $(mysql)
ports:; @$(foreach _,$(serial),echo $(call ports.sql,$_) | $(mysql);)
players:; @$(foreach _,$(serial),echo $(call players.sql,$_) | $(mysql);)
tables:; @$(foreach _,$(serial),echo $(call tables.sql,$_) | $(mysql);)
servers:; @find $(CONFDIR)/poker.server* | xargs grep resthost
