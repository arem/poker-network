# Copyright (C) 2006 Loic Dachary <loic@gnu.org>
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
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
#
XSLTPROC = xsltproc

ECHO	 = echo

ENPAGES = $(shell find * -regex '.*\.en\.xhtml' -print | sed "s/xhtml$$/html/")

all: $(ENPAGES)

$(ENPAGES): %.html: %.xhtml pokersource.xsl navigation.en.xsl
	@echo "Building $@ ..."; \
	filebase=`expr $< : '\(.*\).xhtml'` ; \
	root=`dirname $< | perl -pe 'chop; s:([^/]+):..:g if($$_ ne ".")'` ; \
	xsltproc --novalid \
		--stringparam root $$root --stringparam filebase $$filebase \
		pokersource.xsl $< > $$filebase.html-temp && \
		mv $$filebase.html-temp $$filebase.html

# remove html files for which an xhtml version exists (exclude fr/)
clean:
	rm -f $(ENPAGES)



