////////////////////////////////////////////////////////////////////////////////
//
//     Copyright (C) 2008 Bruno Garnier <bruno.garnier@gmail.com>
//
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License
//     along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

package aspoker.com.bubsy.poker.client.model
{

import aspoker.com.bubsy.poker.client.PokerClient;
import aspoker.com.bubsy.poker.client.communication.JsonStreamProxy;
import aspoker.com.bubsy.poker.client.event.LoginEvent;
import aspoker.com.bubsy.poker.client.event.TableEvent;
import aspoker.com.bubsy.poker.client.util.PollTimer;

import com.adobe.serialization.json.JSON;
import com.bubzy.utils.Logger;

import flash.events.TimerEvent;

public class Table extends PollTimer
{
    private var _gameID:int = 0;
    private var _seats:Array /*[0,0,0,0,0,0,0,0,0,0]*/ = [];
    private var _players:Array /* of Player */ = [];
    private var _user:User = PokerClient.user;
    private var _playerSerialsInGame:Array /* of player serial */ = [];

    private var _actionJsonStream:JsonStreamProxy;

    /* table properties*/
    private var _name:String;
    private var _variant:String;
    private var _percent_flop:int;
    private var _betting_structure:String;
    private var _average_pot:int;
    private var _muck_timeout:int;
    private var _hands_per_hour:int;
    private var _tourney_serial:int;
    private var _numSeats:int;
    private var _numObservers:int;
    private var _player_timeout:int;
    private var _currency_serial:int;
    private var _skin:String

    private var _currentPosition:int = -1;
    private var _previousPosition:int = -1;

    private var _currentPot:Number = 0;
    private var _currentBlind:Number = 0;
    private var _boardCards:Array = []; /*Array of Cards*/

    /*dealer position*/
    private var _dealer:int;
    private var _previous_dealer:int;

    /*buying limits*/
    private var _buyInLimitMin:Number;
    private var _buyInLimitbest:Number;
    private var _rebuy_min:Number;
    private var _buyInLimitmax:Number;

    /* table states */
    private var _tableState:String = TABLE_STATE_BLIND;

    //--------------------------------------------------------------------------
    //
    //  Class constants
    //
    //--------------------------------------------------------------------------

    public const TABLE_STATE_BLIND:String = "blind";
    public const TABLE_STATE_PRE_FLOP:String = "pre-flop";
    public const TABLE_STATE_FLOP:String = "flop";
    public const TABLE_STATE_TURN:String = "turn";
    public const TABLE_STATE_RIVER:String = "river";
    public const TABLE_STATE_END:String = "end";

    public function Table(gameId:int)
    {
        stopPoll();
        _actionJsonStream = PokerClient.stream;
        _gameID = gameId;
        JsonStreamProxy.register(this);

    }

    public function _PacketPokerBoardCards(packet:Object):void
    {
        _boardCards = [];

        for each( var cardNumber:Number in packet.cards)
        {
            _boardCards.push(new Card(cardNumber));
        }

        dispatchEvent(
            new TableEvent(
                TableEvent.PacketPokerBoardCards,
                packet
            )
        );
    }

    public function _onPacketPokerDealer(packet:Object):void
    {
        trace(JSON.encode(packet));
        _dealer = packet.dealer;
        _previous_dealer = packet.previous_dealer;
    }

    private function _gameEnded():void
    {

    }

    public function get boardCards():Array /* of Cards */
    {
        return _boardCards;
    }

    /* return state (constant) */
    public function get state():String
    {
        return _tableState;
    }

    public function get currentBlind():Number
    {
        return _currentBlind;
    }

    private function _newGame():void
    {
        _currentPosition = -1;
        _previousPosition = -1;
        _currentBlind = 0;
        _tableState = TABLE_STATE_BLIND;
        _currentPot = 0;
        resetPlayerBets(getPlayersInGame());
        setPlayersState(getPlayersInGame(),PlayerState.SITIN);
    }

    public function getPlayersInGame():Array /*of players*/
    {
        var arrOfplayers:Array=[];

        for each(var serial:String in _playerSerialsInGame)
        {
            arrOfplayers.push(_players[serial]);
        }

        return arrOfplayers;
    }

    /* get player in game and no fold */
    public function getInvolvedPlayers():Array /*of serials*/
    {
        var arrOfplayers:Array=[];

        for each(var serial:String in _playerSerialsInGame)
        {
            var player:Player = _players[serial];
            if (player.action!=PlayerState.FOLD)
            {
                arrOfplayers.push(serial);
            }
        }
        return arrOfplayers;
    }

    private function setPlayersState(arrOfplayers:Array/*of players*/,
        playerState:PlayerState
    ):void
    {
        for each(var player:Player in arrOfplayers)
        {
            player.action = playerState;
        }
    }

    private function resetPlayerBets(arrOfplayers:Array/*of players*/):void
    {
        for each(var player:Player in arrOfplayers)
        {
            player.bet = 0;
        }
    }

    public function _onPacketPokerStart(packet:Object):void
    {
        /*{"level":0,"cookie":"","hands_count":21,"type":"PacketPokerStart",
        "hand_serial":114,"time":85037.520292,"serial":0,"game_id":321}*/

        _newGame();

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerStart,
                packet
            )
        );
    }

    public function _onPacketPokerState(packet:Object):void
    {
     //   trace(evt.packet.string);

        _tableState = packet.string;

        if (state != TABLE_STATE_PRE_FLOP)
        {
            resetPlayerBets(getPlayersInGame());
            _currentBlind = 0;
        }

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerState,
                packet
            )
        );
    }
    
    public function onPacketPokerChat(packet:Object):void
    {
        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerChat,
                packet
            )
        );
    }

    public function _onPacketPokerPlayerCards(packet:Object):void
    {
        var player:Player = _players[packet.serial] as Player
        player.cards = [];

        for each(var cardNumber:Number in packet.cards)
        {
            if (cardNumber!=255)
            {
                player.cards.push(new Card(cardNumber & 0x3F));
            } else {
                /*back card*/
                player.cards.push(new Card(52));
            }
        }

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerPlayerCards,
                packet
            )
        );
    }

    public function _onPacketPokerBlind(packet:Object):void
    {
        var player:Player = _players[packet.serial] as Player

        if (_currentBlind == 0)
        {
            trace(player.name + "pays the small blind");
            player.action = PlayerState.PAID_SMALL_BLIND;
        } else if (_currentBlind <  packet.amount ) {
            trace(player.name + "pays the big blind");
            player.action = PlayerState.PAID_BIG_BLIND;
        } else {
            trace(player.name + "pays the big blind");
        }

        player.action.amount = packet.amount;
        player.bet = packet.amount;
        _currentBlind = packet.amount;
        _currentPot +=  packet.amount;

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerBlind,
                packet
            )
        );
    }

    public function _onPacketPokerCall(packet:Object):void
    {
        var player:Player = _players[packet.serial] as Player
        player.action = PlayerState.CALL;
        player.action.amount = _currentBlind;

        var blindToCall:Number = _currentBlind - player.bet;
        player.bet+= blindToCall;
        _currentPot +=  blindToCall;

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerCall,
                packet
            )
        );
    }

    public function _onPacketPokerCheck(packet:Object):void
    {
        var player:Player = _players[packet.serial] as Player
        player.action = PlayerState.CHECK;

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerCheck,
                packet
            )
        );
    }

    public function _onPacketPokerFold(packet:Object):void
    {
        var player:Player = _players[packet.serial] as Player
        player.action = PlayerState.FOLD;

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerFold,
                packet
            )
        );
    }

    public function _onPacketPokerRaise(packet:Object):void
    {
        var player:Player = _players[packet.serial] as Player
        player.action = PlayerState.RAISE;
        player.action.amount = packet.amount;
        _currentBlind += player.action.amount;
        player.bet += player.action.amount;
        _currentPot += packet.amount;

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerRaise,
                packet
            )
        );
    }

    public function get currentPot():Number
    {
        return _currentPot;
    }

    public function _onPokerPosition(packet:Object):void
    {
        _previousPosition = _currentPosition;
        _currentPosition =  packet.position;

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerPosition,
                packet
            )
        );
    }

    public function _onPacketPokerBlindRequest(packet:Object):void
    {
        _currentBlind = packet.amount;

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerBlindRequest,
                packet
            )
        );
    }

    public function _onPokerInGame(packet:Object):void
    {
        _playerSerialsInGame = packet.players;

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerInGame,
                packet
            )
        );
    }

    public function _onBuyInLimits(packet:Object):void
    {
        var BuyInLimits:Object = packet;

        _buyInLimitMin = BuyInLimits.min;
        _buyInLimitbest = BuyInLimits.best;
        _rebuy_min = BuyInLimits.rebuy_min;
        _buyInLimitmax = BuyInLimits.max;
    }

    public function _onSitOut(packet:Object):void
    {
        var player:Player = _players[packet.serial] as Player
        player.action = PlayerState.SITOUT;

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerSitOut,
                packet
            )
        );
    }

    public function _onSit(packet:Object):void
    {
        var player:Player = _players[packet.serial] as Player
        player.action = PlayerState.SITIN;

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerSit,
                packet
            )
        );
    }

    public function _onPlayerStats(evt:TableEvent):void
    {
        var player:Player = _players[evt.packet.serial] as Player
        player.setStats(evt.packet);

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerPlayerStats,
                evt.packet
            )
        );
    }

    public function _onPlayerChips(packet:Object):void
    {
        var player:Player = _players[packet.serial] as Player
        player.setChips(packet);

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerPlayerChips,
                packet
            )
        );
    }

    public function destroy():void
    {
        stopPoll();

        _players = null;
    }

    public function get playerSerialsInGame():Array
    {
        return _playerSerialsInGame;
    }

    public function get numSeats():int
    {
        return _numSeats;
    }

    public function get currentPosition():int
    {
        return _currentPosition;
    }

    public function get previousPosition():int
    {
        return _previousPosition;
    }

    public function get BuyInLimitMin():int
    {
        return _buyInLimitMin;
    }

    public function get BuyInLimitbest():int
    {
        return _buyInLimitbest;
    }

    public function get BuyInLimitmax():int
    {
        return _buyInLimitmax;
    }

    public function get gameId():int
    {
        return _gameID;
    }

    public function getPlayerFromSeat(seat:int):Player
    {
        return _players[_seats[seat]];
    }

    public function getPlayer(serial:int):Player
    {
        return _players[serial];
    }

    public function getSerialFromPosition(position:int):int
    {
        if (position==-1) return -1

        return _playerSerialsInGame[position];
    }

    public function _onPacketPokerPlayerArrive(packet:Object):void
    {
        _seats[packet.seat] = packet.serial;
        _players[packet.serial] = new Player(packet);
        _players[packet.serial].action = PlayerState.SITOUT

        dispatchEvent(
            new TableEvent(
            TableEvent.onPacketPokerPlayerArrive,
            packet
            )
        );
    }

    public function _onPacketPokerPlayerLeave(packet:Object):void
    {
        _players[packet.seat] = null;
       
        dispatchEvent(
            new TableEvent(
            TableEvent.onPacketPokerPlayerLeave,
            packet
            )
        );
    }

    public function _onPacketAuthRequest(evt:LoginEvent):void
    {
        dispatchEvent(
            new LoginEvent(
            LoginEvent.onPacketAuthRequest
            )
        );
    }

/* PacketPokerSeats was deprecated when the seat number was added to the
 PlayerArrive and PlayerLeave packets

 public function _onPacketPokerSeats(packet:Object):void
    {
        _seats = packet.seats;

        dispatchEvent(
            new TableEvent(
            TableEvent.onPacketPokerSeats,
            packet
            )
        );
*
    }
*/

    public function get seats():Array
    {
        return _seats;
    }

    public function _onPacketPokerTable(packet:Object):void
    {
        _name = packet.name;
        _variant = packet.variant;
        _percent_flop = packet.percent_flop;
        _betting_structure = packet.betting_structure;
        _average_pot = packet.average_pot;
        _muck_timeout = packet.muck_timeout;
        _hands_per_hour = packet.hands_per_hour;
        _tourney_serial = packet.tourney_serial;
        _numSeats = packet.seats;
        _numObservers = packet.observers;
        _player_timeout = packet.player_timeout;
        _currency_serial = packet.currency_serial;
        _skin = packet.skin;

        _resetSeats(_numSeats);
    
        dispatchEvent(
             new TableEvent(
                    TableEvent.onPacketPokerTable,
                    null
             )
        );
    }
    
    private function _resetSeats(maxSeats:int=0):void
    {
        _seats = [] ;
       
        for (var i:int=0;i<maxSeats;i++) 
        {
            _seats[i] = 0 ; 
        }
    }

    public function quit():void
    {

      if (_user.userSerial == -1 || !_players[_user.userSerial] )
        {/* user not logged or without seat */
       } else {
        /* user logged */
             JsonStreamProxy.unregister(this);
             _actionJsonStream.quit(_gameID);
        }

        dispatchEvent(
                new TableEvent(
                    TableEvent.onPacketPokerPlayerLeave,
                    null
                )
            );
    }

    public function join(gameId:int):void
    {
        startPoll();
        _gameID = gameId;
        _actionJsonStream.tableJoin(gameId,_user.userSerial);
    }

    public function seat(seat:int):void
    {
         trace("je clique testtttttttt");
         _actionJsonStream.seat(_gameID,_user.userSerial,seat);
    }

    public function buyIn(amount:Number):void
    {
        _actionJsonStream.buyIn(_gameID,_user.userSerial,amount);
    }

    public function sitIn():void
    {
        _actionJsonStream.autoBuildAnt(gameId,_user.userSerial);
        _actionJsonStream.sit(_gameID,_user.userSerial);
    }

    public function sitOut():void
    {
        _actionJsonStream.sitOut(_gameID,_user.userSerial);
    }

    public function call():void
    {
        _actionJsonStream.call(_gameID,_user.userSerial);
    }

    public function check():void
    {
        _actionJsonStream.check(_gameID,_user.userSerial);
    }

    public function blind():void
    {
        _actionJsonStream.blind(_gameID,_user.userSerial);
    }

    public function raise(amount:int):void
    {
        _actionJsonStream.raise(_gameID,_user.userSerial,amount);
    }

    public function fold():void
    {
        _actionJsonStream.fold(_gameID,_user.userSerial);
    }

    public function poll():void
    {
        Logger.log("poll gameID:" + _gameID);
        _actionJsonStream.Poll(_gameID);
    }

    protected override function doStep(evt:TimerEvent):void
    {
        poll();
    }
}
}
