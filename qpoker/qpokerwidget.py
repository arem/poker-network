import sys
import unittest
from PyQt4.QtGui import QApplication, QWidget, QGraphicsScene, QGraphicsView, QGraphicsSimpleTextItem, QGraphicsItemAnimation
from PyQt4.QtCore import QTimeLine, QPointF
from PyQt4.QtSvg import QGraphicsSvgItem, QSvgRenderer
from PyQt4.QtOpenGL import QGLWidget, QGLFormat, QGL

def card2SvgElement(card):
    if card == None:
        return "back"
    cardsuit2svg = {"d" : "diamond", "c" : "club", "s" : "spade", "h" : "heart"}
    cardvalue2svg = {"Q" : "queen", "K" : "king", "J" : "jack", "T" : "10", "A" : "1"}
    return (cardvalue2svg.has_key(card[0]) and cardvalue2svg[card[0]] or card[0]) + "_" + cardsuit2svg[card[1]]

class AnimatedGraphicsSvgItem(QGraphicsSvgItem):
    def __init__(self, file):
        QGraphicsSvgItem.__init__(self, file)
        self.animation = QGraphicsItemAnimation()
        self.animation.setItem(self)
        self.timeline = QTimeLine(300)
        self.animation.setTimeLine(self.timeline)
    def setMatrix(self, matrix):
        self.matrix = matrix
        QGraphicsSvgItem.setMatrix(self, matrix)
    def show(self):
        self.scale(0.0, 0.0)
        self.animation.setPosAt(0.0, QPointF(self.matrix.dx()+self.boundingRect().width()/8, self.matrix.dy()+self.boundingRect().height()/8))
        self.animation.setPosAt(1.0, QPointF(self.matrix.dx(), self.matrix.dy()))
        self.animation.setScaleAt(0.0, 0.25, 0.25)
        self.animation.setScaleAt(1.0, 0.5, 0.5)
        self.timeline.start()
        QGraphicsSvgItem.show(self)

class QPokerWidget(QWidget):
    def __init__(self, parent = None):
        QWidget.__init__(self, parent)
        self.renderer = QSvgRenderer("poker.svg")
        self.scene = QGraphicsScene()
        self.chat = QGraphicsSimpleTextItem()
        self.table = QGraphicsSvgItem("poker.svg")
        self.table.setSharedRenderer(self.renderer)
        self.table.setElementId("table")
        self.table.setMatrix(self.renderer.matrixForElement("transform_table"))
        self.scene.addItem(self.chat)
        self.scene.addItem(self.table)
        self.board = []
        for i in range(5):
            card = AnimatedGraphicsSvgItem("svg-cards.svg")
            card.setElementId("back")
            card.setMatrix(self.renderer.matrixForElement("transform_card%i" % i))
            card.setFlag(QGraphicsSvgItem.ItemIsMovable, True)
            card.scale(0.5, 0.5)
            card.hide()
            self.scene.addItem(card)
            self.board.append(card)
        self.names = []
        for i in range(10):
            name = QGraphicsSimpleTextItem()
            self.scene.addItem(name)
            self.names.append(name)
        self.view = QGraphicsView(self)
        self.view.setScene(self.scene)
        self.view.resize(800, 600)
    def renderChat(self, message):
        self.chat.setText(message)
    def renderBoard(self, cards):
        for i in range(len(cards)):
            item = self.board[i]
            card = cards[i]
            item.setElementId(card2SvgElement(card))
            if item.isVisible() == False:
                item.show()
        for i in range(len(cards), len(self.board)):
            item = self.board[i]
            item.hide()
            item.setElementId('back')
    def renderStart(self):
        map(lambda svgItem: svgItem.setElementId('back'), self.board)
        map(lambda svgItem: svgItem.hide(), self.board)
    def renderPlayerArrive(self, seat, name):
        self.names[seat].setText(name)
        self.names[seat].show()
    def renderPlayerLeave(self, seat):
        self.names[seat].setText('')
        self.names[seat].hide()
    def keyPressEvent(self, event):
        if event.text() == "q":
            self.view.scale(1.1, 1.1)
        elif event.text() == "a":
            self.view.scale(0.9, 0.9)

if __name__ == '__main__':
    class QInteractivePokerWidget(QPokerWidget):
        i = 0
        def keyPressEvent(self, event):
            board = ("As", "Ac", "Ad", "Ks", "Kc")
            self.renderBoard(board[:self.i])
            self.i = ((self.i) + 1) % 6
    app = QApplication(sys.argv)
    widget = QInteractivePokerWidget()
    widget.show()
    sys.exit(app.exec_())
