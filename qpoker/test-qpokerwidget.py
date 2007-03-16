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
import unittest
from qpokerwidget import QPokerWidget, card2SvgElement
from PyQt4.QtGui import QApplication

class KeyPressEventMockup:
    def __init__(self, text):
        self.text_ = text
    def text(self):
        return self.text_

class QPokerWidgetTestCase(unittest.TestCase):
    def setUp(self):
        self.widget = QPokerWidget()
    def tearDown(self):
        self.widget = None
    def testWidgetDefaultState(self):
        for card in self.widget.board:
            self.assertEquals(False, card.isVisible())
    def testRenderChat(self):
        self.widget.renderChat("salut les aminches")
        self.assertEquals("salut les aminches", str(self.widget.chat.text()))
    def testRenderBoard(self):
        cards = ("As", "2c", "Qh", "Jd")
        ids = ("1_spade", "2_club", "queen_heart", "jack_diamond")
        map(lambda card, id: self.assertEquals(id, card2SvgElement(card)), cards, ids)            
        self.widget.renderBoard(cards)
        for i in range(len(cards)):
            item = self.widget.board[i]
            id = ids[i]
            self.assertEquals(True, item.isVisible())
            self.assertEquals(id, item.elementId())
        for i in range(len(cards), len(self.widget.board)):
            item = self.widget.board[i]
            self.assertEquals(False, item.isVisible())
            self.assertEquals('back', item.elementId())
    def testRenderEmptyBoard(self):
        cards = []
        self.widget.renderBoard(cards)
        map(lambda svgItem: self.assertEquals(False, svgItem.isVisible()), self.widget.board)
        map(lambda svgItem: self.assertEquals('back', svgItem.elementId()), self.widget.board)
    def testRenderStart(self):
        self.widget.renderStart()
        map(lambda svgItem: self.assertEquals(False, svgItem.isVisible()), self.widget.board)
        map(lambda svgItem: self.assertEquals('back', svgItem.elementId()), self.widget.board)
    def testRenderPlayerArrive(self):
        self.widget.renderPlayerArrive(0, "playerName")
        self.assertEquals('playerName', self.widget.names[0].text())
        self.assertEquals(True, self.widget.names[0].isVisible())
    def testRenderPlayerLeave(self):
        self.widget.renderPlayerLeave(0)
        self.assertEquals('', self.widget.names[0].text())
        self.assertEquals(False, self.widget.names[0].isVisible())
        self.assertEquals(False, self.widget.moneys[0].isVisible())
        self.assertEquals(False, self.widget.bets[0].isVisible())
    def testRenderPlayerChips(self):
        self.widget.renderPlayerChips(0, 10000, 100)
        self.assertEquals('10000', self.widget.moneys[0].text())
        self.assertEquals(True, self.widget.moneys[0].isVisible())
        self.assertEquals('100', self.widget.bets[0].text())
        self.assertEquals(True, self.widget.bets[0].isVisible())
        self.widget.renderPlayerChips(0, 10000, 0)
        self.assertEquals(False, self.widget.bets[0].isVisible())
    def testRenderPot(self):
        self.widget.renderPot(0, 10000)
        self.assertEquals('10000', self.widget.pots[0].text())
        self.assertEquals(True, self.widget.pots[0].isVisible())
        self.widget.renderPotReset()
        self.assertEquals('', self.widget.pots[0].text())
        self.assertEquals(False, self.widget.pots[0].isVisible())
    def testRenderPosition(self):
        position = 1
        self.widget.renderPosition(position)
        for i in range(10):
            if i == position:
                self.assertEquals('seat_inposition', str(self.widget.seats[i].elementId()))
            else:
                self.assertEquals('seat', str(self.widget.seats[i].elementId()))
        self.widget.renderPositionReset()
        for seat in self.widget.seats:
            self.assertEquals('seat', str(seat.elementId()))
    def testKeyPressEventZoomIn(self):
        matrix = self.widget.view.matrix()
        matrix.scale(1.1, 1.1)
        self.widget.keyPressEvent(KeyPressEventMockup('q'))
        self.assertEquals(matrix, self.widget.view.matrix())
    def testKeyPressEventZoomOut(self):
        matrix = self.widget.view.matrix()
        matrix.scale(0.9, 0.9)
        self.widget.keyPressEvent(KeyPressEventMockup('a'))
        self.assertEquals(matrix, self.widget.view.matrix())
    def testSeatClickedEvent(self):
        self.seatClicked = None
        def f(seat):
            self.seatClicked = seat
        self.widget.seatClicked = f
        self.widget.seats[0].mousePressEvent(None)
        self.assertEquals(0, self.seatClicked)
    def testFoldClickedEvent(self):
        self.foldClicked = None
        def f():
            self.foldClicked = True
        self.widget.foldClicked = f
        self.widget.fold.mousePressEvent(None)
        self.assertEquals(True, self.foldClicked)
    def testCheckClickedEvent(self):
        self.checkClicked = None
        def f():
            self.checkClicked = True
        self.widget.checkClicked = f
        self.widget.check.mousePressEvent(None)
        self.assertEquals(True, self.checkClicked)
    def testCallClickedEvent(self):
        self.callClicked = None
        def f():
            self.callClicked = True
        self.widget.callClicked = f
        self.widget.call.mousePressEvent(None)
        self.assertEquals(True, self.callClicked)
    def testBetClickedEvent(self):
        self.betClicked = None
        def f():
            self.betClicked = True
        self.widget.betClicked = f
        self.widget.bet.mousePressEvent(None)
        self.assertEquals(True, self.betClicked)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    unittest.main()
