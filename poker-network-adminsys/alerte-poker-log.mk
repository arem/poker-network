top:; @date

SHELL := /bin/bash

host := $(shell hostname -s)
self := $(firstword $(MAKEFILE_LIST))
base := $(basename $(notdir $(self)))

$(self):;

# defaults args

WHO = root@localhost

NLOGS = 5

# cron step
FREQ := 30
FREQ_S := $(FREQ)
FREQ_P := $(FREQ)

# try to avoid mail same error twice
MINUTES = $(shell expr $(FREQ) + 5)

ifeq ($(host),poker1)
MAX_S := 0
MAX_P := 25
else ifeq ($(host),poker2)
MAX_S := 5
MAX_P := 25
else
$(error where are you ?)
endif

ifeq ($(MAKECMDGOALS),server)
MAX := $(MAX_S)
else ifeq ($(MAKECMDGOALS),proxy)
MAX := $(MAX_P)
else
MAX := 0
endif

# end defaults

log := /var/log
log.server = $(log)/poker-network-server*.log
log.proxy = $(log)/poker-network-proxy*.log

targets := server proxy

# get recent log file names
files := ls $${file}* | sort -n -t. -k3 | tac | tail -$(NLOGS)
# fold line starting with a tab
fold := xargs sed -e $$':a\nN\n/\\n\\t/s//_/\nta\nP\nD\n'
# avoid huge mail
clean := $(if $(filter proxy,$(MAKECMDGOALS)),grep -v Starting.factory |)
# skip old lines
lines := $(clean) awk -F+ -v date="$$(date -d '$(MINUTES) minute ago' '+%F %T')" '{ if ($$1 > date) print }'
# mail lines
mail := mail -s "$(base) == $$CNT during last $(MINUTES) minutes on $(host):$${file}*" $(WHO)
# count matching lines test against max and mail
tmp = /tmp/$(base)-$@
test = (CNT=$$(grep -c Traceback); test $$CNT -gt $(MAX) && $(mail) < $(tmp) || :)

ifndef DEBUG

$(targets):; @ls $(log.$@) | while read file; do $(files) | $(fold) | $(lines) | tee $(tmp) | $(test); done
.PHONY: $(targets)

else # do it step by step
$(error broken, no outer loop)

.stone:; touch $@
.PHONY: .stone

$(targets): % : %/lines; CNT=$$(grep -c Traceback $<); test $$CNT -gt $(MAX) && $(mail) < $<

%/files: .stone  %/.mkdir; ls $(log.$(@D)) | $($(@F)) > $@
%/fold: %/files; < $< $($(@F)) > $@
%/lines: %/fold; < $< $($(@F)) >$@
.PRECIOUS: %/files %/fold
endif

bin := /usr/local/bin

cron := $(base).cron
cron: $(cron);

server := S
proxy := P
cron.line = '*/$(FREQ_$($_)) * * * * root \t$(bin)/$(base).mk MAX=$(MAX_$($_)) MINUTES=$(MINUTES) $_'
$(cron): $(self)
	@echo -e 'MAILTO=root\n' >> $@
	@$(foreach _,$(targets),echo -e $(cron.line) >> $@;)

user := root
group := staff
install.cmd = install --backup=numbered -o $(user) -g $(group) -m $1 $< $@
cron.d := /etc/cron.d
$(bin)/$(self): $(self); $(call install.cmd,555)
$(cron.d)/$(base): $(cron); $(call install.cmd,444)
install: $(bin)/$(self) $(cron.d)/$(base)

####

%/.mkdir:; mkdir -p $(@D); touch $@
