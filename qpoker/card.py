import sys
import unittest
from PyQt4.QtGui import QApplication
from PyQt4.QtSvg import QGraphicsSvgItem
def card2SvgElement(card):
    cardsuit2svg = {"d" : "diamond", "c" : "club", "s" : "spade", "h" : "heart"}
    cardvalue2svg = {"Q" : "queen", "K" : "king", "J" : "jack", "T" : "10", "A" : "1"}
    return (cardvalue2svg.has_key(card[0]) and cardvalue2svg[card[0]] or card[0]) + "_" + cardsuit2svg[card[1]]
class CardTestCase(unittest.TestCase):
    def testCard2SvgElement(self):
        self.assertEquals("10_diamond", card2SvgElement("Td"))
        self.assertEquals("2_spade", card2SvgElement("2s"))
        self.assertEquals("queen_club", card2SvgElement("Qc"))
        self.assertEquals("1_heart", card2SvgElement("Ah"))

if __name__ == '__main__':
    app = QApplication(sys.argv)
    unittest.main()
