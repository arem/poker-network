import sys
import string
from PyQt4.QtGui import QApplication, QWidget, QGraphicsScene, QGraphicsView, QGraphicsTextItem
from PyQt4.QtSvg import QGraphicsSvgItem
from PyQt4.QtOpenGL import QGLWidget, QGLFormat, QGL
import qt4reactor
qt4reactor.install()
from twisted.internet import reactor
from pokernetwork.pokerclient import PokerClientFactory
from pokernetwork.pokernetworkconfig import Config
from pokerui.pokerdisplay import PokerDisplay
from pokerui.pokerrenderer import PokerRenderer
from pokerui.pokerinterface import PokerInterface
from pokernetwork.pokerpackets import PACKET_POKER_CHAT, PACKET_POKER_BOARD_CARDS, PACKET_POKER_START

class GraphicsView(QGraphicsView):
    def keyPressEvent(self, event):
        if event.text() == "q":
            self.scale(1.1, 1.1)
        elif event.text() == "a":
            self.scale(0.9, 0.9)

class DummyPokerDisplay(PokerDisplay):
    def __init__(self, *args, **kwargs):
        PokerDisplay.__init__(self, *args, **kwargs)
        self.scene = QGraphicsScene()
        self.chat = QGraphicsTextItem()
        self.chat.setFlag(QGraphicsTextItem.ItemIsMovable, True)
        self.table = QGraphicsSvgItem("table.svg")
        self.table.setFlag(QGraphicsSvgItem.ItemIsMovable, True)
        self.board = []
        for i in range(5):
            card = QGraphicsSvgItem("svg-cards.svg", self.table)
            card.setElementId("back")
            card.setFlag(QGraphicsSvgItem.ItemIsMovable, True)
            card.scale(0.5, 0.5)
            self.board.append(card)
        self.scene.addItem(self.chat)
        self.scene.addItem(self.table)
        self.view = GraphicsView()
        self.view.setScene(self.scene)
        self.view.resize(800, 600)
        self.view.show()

    def render(self, packet):
        print "PokerDisplay2D::render: " + str(packet)
        if packet.type == PACKET_POKER_CHAT:
            message = packet.message
            message = message.replace('\n', '')
            self.chat.setPlainText(message)
        elif packet.type == PACKET_POKER_BOARD_CARDS:
            game = self.factory.getGame(packet.game_id)
            board = game.eval.card2string(packet.cards)
            def card2SvgElement(card):
                cardsuit2svg = {"d" : "diamond", "c" : "club", "s" : "spade", "h" : "heart"}
                cardvalue2svg = {"Q" : "queen", "K" : "king", "J" : "jack", "T" : "10", "A" : "1"}
                return (cardvalue2svg.has_key(card[0]) and cardvalue2svg[card[0]] or card[0]) + "_" + cardsuit2svg[card[1]]
            for i in range(len(board)):
                elementId = card2SvgElement(board[i])
                self.board[i].setElementId(elementId)
        elif packet.type == PACKET_POKER_START:
            for card in self.board:
                card.setElementId("back")
            
        #PokerPlayer2D::render: type = POKER_BOARD_CARDS(62) serial = 0 game_id = 100 cards = [36, 0, 5]
        #PokerPlayer2D::render: type = POKER_CHAT(86) serial = 0 game_id = 100 message = Dealer: BOTbytpet checks 
class DummyPokerRenderer(PokerRenderer):
    def __init__(self, *args, **kwargs):
        PokerRenderer.__init__(self, *args, **kwargs)
class DummyPokerInterface(PokerInterface):
    def __init__(self, *args, **kwargs):
        self.verbose = 5
        PokerInterface.__init__(self, *args, **kwargs)
    def command(self, *args):
        print "DummyPokerInterface.command %s" % str(args)
        if args[0] == "login":
            #call later to allow callback to be attached
            reactor.callLater(0, lambda: self.event("login", "ok", "proppy", "pqsspqss", "1"))
        elif args[0] == "menu":
            pass
        elif args[0] == "chat":
            pass
        elif args[0] == "buy_in":
            pass
        elif args[0] == "sit_actions":
            pass
        elif args[0] == "cashier":
            pass
        elif args[0] == "tournaments":
            pass
        elif args[0] == "lobby":
            if args[1] == "info":
                reactor.callLater(0, lambda: self.event("lobby", "join", "100"))
        else:
            print "*ERROR* command not implemented"
            raise UserWarning
        
class DummyPokerClientFactory(PokerClientFactory):
    def __init__(self):
        settings = Config([''])
        settings.load('poker2d-test.xml')
        config = Config([''])
        config.load('client.xml')
        self.verbose = 5
        PokerClientFactory.__init__(self, settings = settings, config = config)
        self.display = DummyPokerDisplay(settings = settings,
                                         config = config,
                                         factory = self)
        self.renderer = DummyPokerRenderer(self)
        self.interface = DummyPokerInterface()
        self.renderer.interfaceReady(self.interface)
    def buildProtocol(self, addr):
        protocol = PokerClientFactory.buildProtocol(self, addr)
        self.display.setProtocol(protocol)
        self.renderer.setProtocol(protocol)
        return protocol

if __name__ == '__main__':
    app = QApplication(sys.argv)
    client = DummyPokerClientFactory()
    reactor.connectTCP("poker-tst.pok3d.free.tld", 19380, client)
    reactor.run()

