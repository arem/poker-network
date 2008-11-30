#!/usr/bin/env python
#
#Copyright (c) 2008 Thomas Johnson <tomfmason@gmail.com>
#
#Permission is hereby granted, free of charge, to any person
#obtaining a copy of this software and associated documentation
#files (the "Software"), to deal in the Software without
#restriction, including without limitation the rights to use,
#copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the
#Software is furnished to do so, subject to the following
#conditions:
#
#The above copyright notice and this permission notice shall be
#included in all copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
#OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
#HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
#WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
#OTHER DEALINGS IN THE SOFTWARE.
#

import web
import json
from sys import stderr
import string
from decimal import *

urls=("/", "ICMWebService")
class ICM:
	def __init__(self, stacks, payouts):
		self.stacks = stacks
		self.payouts = payouts
		self.equities = []
		self.prepare()
	def prepare(self):
		total = sum(self.stacks)
		for k,v in enumerate(stacks):
			self.equities.append(round(Decimal(str(self.getEquities(total, k,0))),4))
	def getEquities(self, total, player, depth):
		D = Decimal
		eq = D(self.stacks[player]) / total * D(str(self.payouts[depth]))
		if(depth + 1 < len(self.payouts)):
			i=0
			for stack in self.stacks:
				if i != player and stack > 0.0:
					self.stacks[i] = 0.0
					eq += self.getEquities((total - stack), player, (depth + 1)) * (stack / D(total))
					self.stacks[i] = stack
				i += 1
		return eq
class ICMWebService:
	def GET(self):
		input = web.input()
		web.write("Content-type: text/plain\n\n")
		stacks=json.read(input["stacks"])
		payouts=json.read(input["payouts"])
		icm = ICM(stacks,payouts)
		print json.write(icm.equities)



if __name__ == "__main__": web.run(urls, globals())
