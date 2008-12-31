#!/usr/bin/env python
#
# Copyright (C) 2007, 2008 Johan Euphrosine
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

from pokereval import PokerEval
import json
import web
from sys import stderr
import string

urls=('/', 'PokerEvalWebService')
#urls=('/pokerodds', 'PokerEvalWebService')

gameparams_table = [
    ["holdem",    2,  2,  5, 0, 1, "Holdem Hi"],
    ["holdem8",   2,  2,  5, 1, 1, "Holdem Hi/Low 8-or-better"],
    ["omaha",     4,  4,  5, 0, 1, "Omaha Hi"],
    ["omaha8",    4,  4,  5, 1, 1, "Omaha Hi/Low 8-or-better"],
    ["7stud",     3,  7,  0, 0, 1, "7-card Stud Hi"],
    ["7stud8",    3,  7,  0, 1, 1, "7-card Stud Hi/Low 8-or-better"],
    ["7studnsq",  3,  7,  0, 1, 1, "7-card Stud Hi/Low no qualifier"],
    ["razz",      3,  7,  0, 1, 0, "Razz (7-card Stud A-5 Low)"],
    ["5draw",     0,  5,  0, 0, 1, "5-card Draw Hi with joker"],
    ["5draw8",    0,  5,  0, 1, 1, "5-card Draw Hi/Low 8-or-better with joker"],
    ["5drawnsq",  0,  5,  0, 1, 1, "5-card Draw Hi/Low no qualifier with joker"],
    ["lowball",   0,  5,  0, 1, 0, "5-card Draw A-5 Lowball with joker"],
    ["lowball27", 0,  5,  0, 1, 0, "5-card Draw 2-7 Lowball"]]
class GameParam:
    def __init__(self, game, minpocket, maxpocket, maxboard, haslopot, hashipot, name):
        self.game = game
        self.minpocket = minpocket
        self.maxpocket = maxpocket
        self.maxboard = maxboard
        self.haslopot = haslopot
        self.hashipot = hashipot
        self.name = name
gameparams = {}
max_iterations = 750000
for args in gameparams_table:
    param = GameParam(*args)
    gameparams[param.game] = param
def apply_game_params(game, pockets, board, dead, iterations):
    pockets = [pocket[:gameparams[game].maxpocket] for pocket in pockets]
    pockets = [pocket + ["__"] * (gameparams[game].maxpocket - len(pocket))
               for pocket in pockets]
    board += ["__"] * (gameparams[game].maxboard - len(board))
    if iterations and iterations > max_iterations:
        iterations = max_iterations
    return {"game":game, "pockets":pockets, "board":board, "dead":dead, "iterations":iterations}

class PokerEvalWebService:
    def GET(self):
        input = web.input()
        web.header("Content-Type","text/plain");
        defaultParams = {"game":"holdem","pockets":[],"board":[],"dead":[],"iterations":10000}
        kwargs = defaultParams
        jsonp = None
        for key in input:
            value = input[key]
            if key == "jsoncallback":
                jsonp = value
            elif key in ["game", "pockets", "board", "dead", "iterations"]:
                kwargs[key] = json.read(value)
        
        cards = []
        for pocket in kwargs["pockets"]:
            cards.extend(pocket)
        cards.extend(kwargs["board"])
        cards.extend(kwargs["dead"])
        duplicate = set(filter(lambda card: (card != "__" and cards.count(card) > 1), cards))
        if len(duplicate) > 0:
            poker_eval_result = {"error": "duplicate cards: %s" % string.join(duplicate)}
        else:
            params = apply_game_params(**kwargs)
            try:
                poker_eval_result = PokerEval().poker_eval(**params)
            except Exception, e:
                poker_eval_result = {"error": str(e)}
        if jsonp:
            result = "%s(%s)" % (jsonp, json.write(poker_eval_result))
        else:
            result = json.write(poker_eval_result)
        print result

if __name__ == "__main__": web.run(urls, globals())

        
# Interpreted by emacs
# Local Variables:
# compile-command: "cd .. && /bin/sh -x run-test"
# End:
