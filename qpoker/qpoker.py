import sys
import string
import qt4reactor
qt4reactor.install()
from twisted.internet import reactor
from pokernetwork.pokerclient import PokerClientFactory
from pokernetwork.pokernetworkconfig import Config
from pokerui.pokerdisplay import PokerDisplay
from pokerui.pokerrenderer import PokerRenderer
from pokerui.pokerinterface import PokerInterface
from pokernetwork.pokerpackets import PACKET_POKER_CHAT, PACKET_POKER_BOARD_CARDS, PACKET_POKER_START, PACKET_POKER_PLAYER_ARRIVE, PACKET_POKER_PLAYER_LEAVE, PACKET_POKER_PLAYER_CHIPS
from qpokerwidget import QPokerWidget
from PyQt4.QtGui import QApplication

class DummyPokerDisplay(PokerDisplay):
    def __init__(self, *args, **kwargs):
        PokerDisplay.__init__(self, *args, **kwargs)
        self.widget = QPokerWidget()
        self.widget.show()
        self.serial2seat = {}
    def render(self, packet):
        print "PokerDisplay2D::render: " + str(packet)
        if packet.type == PACKET_POKER_CHAT:
            message = packet.message
            message = message.replace('\n', '')
            self.widget.renderChat(message)
        elif packet.type == PACKET_POKER_BOARD_CARDS:
            game = self.factory.getGame(packet.game_id)
            board = game.eval.card2string(packet.cards)
            self.widget.renderBoard(board)
        elif packet.type == PACKET_POKER_START:
            self.widget.renderStart()
        elif packet.type == PACKET_POKER_PLAYER_ARRIVE:
            self.serial2seat[packet.serial] = packet.seat
            self.widget.renderPlayerArrive(packet.seat, packet.name)
        elif packet.type == PACKET_POKER_PLAYER_LEAVE:
            del self.serial2seat[packet.serial]
            self.widget.renderPlayerLeave(packet.seat)
        elif packet.type == PACKET_POKER_PLAYER_CHIPS:
            self.widget.renderPlayerChips(self.serial2seat[packet.serial], packet.money, packet.bet)
            
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

