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
import aspoker.com.bubsy.poker.client.communication.TableJsonStream;
import aspoker.com.bubsy.poker.client.event.LoginEvent;
import aspoker.com.bubsy.poker.client.event.TableEvent;
import aspoker.com.bubsy.poker.client.util.PollTimer;

import com.adobe.serialization.json.JSON;
import com.bubzy.utils.Logger;

import flash.events.TimerEvent;

public class Table extends PollTimer
{
    private var _gameID:int = 0;
    private var _seats:Array=[];/*[0,0,0,0,0,0,0,0,0,0]*/
    private var _players:Array=[]/* of Player */;
    private var _user:User = PokerClient.user;

    private var _actionJsonStream:TableJsonStream = new TableJsonStream();

    /* table info*/
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

    /*buying limits*/
    private var _BuyInLimitMin:Number;
    private var _BuyInLimitbest:Number;
    private var _rebuy_min:Number;
    private var _BuyInLimitmax:Number;

    public function Table(gameId:int)
    {
        stopPoll();
        _gameID = gameId;

        //add Listeners for user action stream
        _actionJsonStream.addEventListener(
          TableEvent.onPacketPokerTable,
          _onPacketPokerTable);

        _actionJsonStream.addEventListener(
            TableEvent.onPacketPokerSeats,
            _onPacketPokerSeats);

        _actionJsonStream.addEventListener(
            TableEvent.onPacketPokerPlayerArrive,
            _onPacketPokerPlayerArrive);

        _actionJsonStream.addEventListener(
            TableEvent.onPacketPokerPlayerLeave,
            _onPacketPokerPlayerLeave);

        _actionJsonStream.addEventListener(
            LoginEvent.onPacketAuthRequest,
            _onPacketAuthRequest);

        _actionJsonStream.addEventListener(
            TableEvent.onPacketPokerPlayerStats,
            _onPlayerStats);

        _actionJsonStream.addEventListener(
            TableEvent.onPacketPokerPlayerChips,
            _onPlayerChips);

        _actionJsonStream.addEventListener(
            TableEvent.onPacketPokerBuyInLimits,
            _onBuyInLimits);

        _actionJsonStream.addEventListener(
            TableEvent.onPacketPokerSit,
            _onSit);

       _actionJsonStream.addEventListener(
            TableEvent.onPacketPokerSitOut,
            _onSitOut);

    }

    private function _onBuyInLimits(evt:TableEvent):void
    {
        var BuyInLimits:Object = evt.packet;
        _BuyInLimitMin = BuyInLimits.min;
        _BuyInLimitbest = BuyInLimits.best;
        _rebuy_min = BuyInLimits.rebuy_min;
        _BuyInLimitmax = BuyInLimits.max;
    }

    private function _onSitOut(evt:TableEvent):void
    {
        var player:Player = _players[evt.packet.serial] as Player
        player.setStats(evt.packet);

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerSitOut,
                evt.packet
            )
        );
    }

    private function _onSit(evt:TableEvent):void
    {
        var player:Player = _players[evt.packet.serial] as Player
        player.setStats(evt.packet);

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerSit,
                evt.packet
            )
        );
    }

    private function _onPlayerStats(evt:TableEvent):void
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

    private function _onPlayerChips(evt:TableEvent):void
    {
        var player:Player = _players[evt.packet.serial] as Player
        player.setChips(evt.packet);

        dispatchEvent(
            new TableEvent(
                TableEvent.onPacketPokerPlayerChips,
                evt.packet
            )
        );
    }

    public function destroy():void
    {
        stopPoll();

        //remove Listeners for user action stream
        _actionJsonStream.removeEventListener(
          TableEvent.onPacketPokerTable,
          _onPacketPokerTable);

        _actionJsonStream.removeEventListener(
            TableEvent.onPacketPokerSeats,
            _onPacketPokerSeats);

        _actionJsonStream.removeEventListener(
            TableEvent.onPacketPokerPlayerArrive,
            _onPacketPokerPlayerArrive);

        _actionJsonStream.removeEventListener(
            TableEvent.onPacketPokerPlayerLeave,
            _onPacketPokerPlayerLeave);

        _players = null;
    }

    public function get numSeats():int
    {
        return _numSeats;
    }

    public function get BuyInLimitMin():int
    {
        return _BuyInLimitMin;
    }

    public function get BuyInLimitbest():int
    {
        return _BuyInLimitbest;
    }

    public function get BuyInLimitmax():int
    {
        return _BuyInLimitmax;
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

    private function _onPacketPokerPlayerArrive(evt:TableEvent):void
    {
        _seats[evt.packet.seat] = evt.packet.serial;
        _players[evt.packet.serial] = new Player(evt.packet);

        dispatchEvent(
            new TableEvent(
            TableEvent.onPacketPokerPlayerArrive,
            evt.packet
            )
        );
    }

    private function _onPacketPokerPlayerLeave(evt:TableEvent):void
    {
      _players[evt.packet.seat] = null;
       dispatchEvent(
            new TableEvent(
            TableEvent.onPacketPokerPlayerLeave,
            evt.packet
            )
        );
    }

    private function _onPacketAuthRequest(evt:LoginEvent):void
    {
        dispatchEvent(
            new LoginEvent(
            LoginEvent.onPacketAuthRequest
            )
        );
    }

    private function _onPacketPokerSeats(evt:TableEvent):void
    {
        _seats = evt.packet.seats;
        dispatchEvent(
            new TableEvent(
            TableEvent.onPacketPokerSeats,
            evt.packet
            )
        );
    }

    public function get seats():Array
    {
        return _seats;
    }

    private function _onPacketPokerTable(evt:TableEvent):void
    {
        var tableInfo:Object = evt.packet;
        _name = tableInfo.name;
        _variant = tableInfo.variant;
        _percent_flop = tableInfo.percent_flop;
        _betting_structure = tableInfo.betting_structure;
        _average_pot = tableInfo.average_pot;
        _muck_timeout = tableInfo.muck_timeout;
        _hands_per_hour = tableInfo.hands_per_hour;
        _tourney_serial = tableInfo.tourney_serial;
        _numSeats = tableInfo.seats;
        _numObservers = tableInfo.observers;
        _player_timeout = tableInfo.player_timeout;
        _currency_serial = tableInfo.currency_serial;
        _skin = tableInfo.skin;
    }

    public function quit():void
    {
        if (_user.userSerial==-1 || !_players[_user.userSerial] )
        {/* user not logged or without seat */
            dispatchEvent(
                new TableEvent(
                    TableEvent.onPacketPokerPlayerLeave,
                    null
                )
            );
        } else {
        /* user logged */
            _actionJsonStream.quit(_gameID);
        }
    }
    public function join(gameId:int):void
    {
        startPoll();
        _gameID = gameId;
        _actionJsonStream.tableJoin(gameId,_user.userSerial);
    }

    public function seat(seat:int):void
    {
        _actionJsonStream.seat(_gameID,_user.userSerial,seat);
    }

    public function buyIn(amount:Number):void
    {
        _actionJsonStream.buyIn(_gameID,_user.userSerial,amount);
    }

    public function sit():void
    {
        _actionJsonStream.sit(_gameID,_user.userSerial);
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