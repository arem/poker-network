#!/usr/bin/python
#
# Copyright (C) 2007 Johan Euphrosine
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
# Authors:
#  Johan Euphrosine <proppy@aminche.com>
#

from distutils.core import setup
setup(name='qpoker',
      version='1.0',
      description='cute pokersource client',
      long_description='qpoker display the poker table in cute (Qt) way, by extensive use of animation and scalable vector graphics.',
      author='Johan Euphrosine (proppy)',
      author_email='proppy@aminche.com',
      url='http://pokersource.info',
      license='GPL',
      platforms=['linux'],
      scripts=['bin/qpoker'],
      packages=['qpoker'],
      data_files=[('share/qpoker', ['poker.svg', 'svg-cards.svg', 'client.xml']),
                  ('share/man/man1', ['qpoker.1'])]
      )
