#!/usr/bin/env python
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
