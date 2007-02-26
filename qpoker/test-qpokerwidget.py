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
    def testRenderChat(self):
        self.widget.renderChat("salut les aminches")
        self.assertEquals("salut les aminches", str(self.widget.chat.toPlainText()))
    def testRenderBoard(self):
        cards = ("As", "2c", "Qh", "Jd", "Ts")
        ids = ("1_spade", "2_club", "queen_heart", "jack_diamond", "10_spade")
        map(lambda card, id: self.assertEquals(id, card2SvgElement(card)), cards, ids)
        self.widget.renderBoard(cards)
        map(lambda id, svgItem: self.assertEquals(id, svgItem.elementId()), ids, self.widget.board)
    def testRenderEmptyBoard(self):
        cards = []
        self.widget.renderBoard(cards)
        map(lambda svgItem: self.assertEquals('back', svgItem.elementId()), self.widget.board)
    def testRenderStart(self):
        self.widget.renderStart()
        map(lambda svgItem: self.assertEquals('back', svgItem.elementId()), self.widget.board)
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

if __name__ == '__main__':
    app = QApplication(sys.argv)
    unittest.main()
