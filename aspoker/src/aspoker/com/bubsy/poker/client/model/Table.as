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
import aspoker.com.bubsy.poker.client.event.TableEvent;
import aspoker.com.bubsy.poker.client.util.PollTimer;

import com.adobe.serialization.json.JSON;
import com.bubzy.utils.Logger;

import flash.events.TimerEvent;

public class Table extends PollTimer
{
    private var tableInfo:Object;
    private var _gameID:int = 0;
    private var _seats:Array=[];
    private var _players:Array=[]/* of Player */;
    private var _user:User = PokerClient.user;

    /*max_players = packet.seats;
    is_tourney
    this.board = [ null, null, null, null, null ];
    this.pots = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ];
    this.buyIn = { min: 1000000000, max: 1000000000, best: 1000000000, bankroll: 0 };
    this.dealer = -1;
    this.position = -1;
    this.state = 'end';
    seat
    */
    private var _actionJsonStream:TableJsonStream = new TableJsonStream();
    private var _pollJsonStream:TableJsonStream = new TableJsonStream();

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
            _onPacketPokerSeat);

        _actionJsonStream.addEventListener(
            TableEvent.onPacketPokerPlayerArrive,
            _onPacketPokerPlayerArrive);

        _actionJsonStream.addEventListener(
            TableEvent.onPacketPokerPlayerLeave,
            _onPacketPokerPlayerLeave);

        //add Listeners for poll stream
       _pollJsonStream.addEventListener(
           TableEvent.onPacketPokerPlayerArrive,
           _onPacketPokerPlayerArrive);

       _pollJsonStream.addEventListener(
           TableEvent.onPacketPokerPlayerLeave,
           _onPacketPokerPlayerLeave);

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
            _onPacketPokerSeat);

        _actionJsonStream.removeEventListener(
            TableEvent.onPacketPokerPlayerArrive,
            _onPacketPokerPlayerArrive);

        _actionJsonStream.removeEventListener(
            TableEvent.onPacketPokerPlayerLeave,
            _onPacketPokerPlayerLeave);

        //remove Listeners for poll stream
        _pollJsonStream.removeEventListener(
           TableEvent.onPacketPokerPlayerArrive,
           _onPacketPokerPlayerArrive);

        _pollJsonStream.removeEventListener(
           TableEvent.onPacketPokerPlayerLeave,
           _onPacketPokerPlayerLeave);

        _players = null;
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

    private function _onPacketPokerSeat(evt:TableEvent):void
    {
        if (_seats && _seats.join() != evt.packet.seats.join())
        {
            _seats = evt.packet.seats;
            dispatchEvent(
                new TableEvent(
                TableEvent.onPacketPokerSeats,
                evt.packet
                )
            );
         }
    }

    public function get seats():Array
    {
        return _seats;
    }

    private function _onPacketPokerTable(evt:TableEvent):void
    {
        tableInfo = evt.packet;
        trace("tableName: " + tableInfo.name);
        trace("tableVariant: " + tableInfo.variant);
        trace("tablePercent_flop: " + tableInfo.percent_flop);
        trace("tableBetting_structure: " + tableInfo.betting_structure);
        trace("tableAverage_pot: " + tableInfo.average_pot);
        trace("tableMuck_timeout: " + tableInfo.muck_timeout);
        trace("tableHands_per_hour: " + tableInfo.hands_per_hour);
        trace("tableTourney_serial: " + tableInfo.tourney_serial);
        trace("tableSeats:" + tableInfo.seats);
        trace("tableObservers: " + tableInfo.observers);
        trace("tablePlayer_timeout: " + tableInfo.player_timeout);
        trace("tableCurrency_serial: " + tableInfo.currency_serial);
        trace("tableSkin: " + tableInfo.skin);
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

    public function buyIn():void
    {
        _actionJsonStream.buyIn(_gameID,_user.userSerial);
    }

    public function poll():void
    {
        Logger.log("poll gameID:" + _gameID);
        _pollJsonStream.Poll(_gameID);
    }

    protected override function doStep(evt:TimerEvent):void
    {
        poll();
    }
}
}