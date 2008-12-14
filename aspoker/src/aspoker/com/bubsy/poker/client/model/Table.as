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

import aspoker.com.bubsy.poker.client.communication.TableJsonStream;
import aspoker.com.bubsy.poker.client.event.TableEvent;

public class Table
{
	private var tableInfo:Object;
	private var _gameID:int = 0;
	private var _seats:Array;
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
    private var _pokerConnection:TableJsonStream = new TableJsonStream();

	public function Table(gameId:int)
	{
 		_pokerConnection.addEventListener(
            TableEvent.onPacketPokerTable,
            _onPacketPokerTable);

		_pokerConnection.addEventListener(
            TableEvent.onPacketPokerSeats,
            _onPacketPokerSeat);

   		join(gameId);

	}

	private function _onPacketPokerSeat(evt:TableEvent):void
	{
		_seats = evt.packet.seats;
		trace(_seats);
		trace("SeatCount: "+ _seats.length);
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

	public function join(gameId:int):void
	{
    	//Logger.log(UserSerial +  " join table" + gameid);
    	_gameID = gameId;
		_pokerConnection.tableJoin(gameId,User.UserSerial);
	}
}
}