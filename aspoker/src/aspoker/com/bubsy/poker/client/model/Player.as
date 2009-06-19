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
    public class Player
    {

      //  public static const STATE_SIT_IN:int = 1;
      //  public static const STATE_SIT_OUT:int = 2;
      //  public static const STATE_WITH_SEAT:int = 3;
      //  public static const STATE_OBSERVER:int = 4;

     //   public var state:int=STATE_OBSERVER;

        private var _name:String;
        private var _url:String;
        private var _auto:Boolean;
        private var _outfit:String;
        private var _wait_for:Boolean;
        private var _buy_in_payed:Boolean;
        private var _serial:int;
        private var _sit_out_next_turn:Boolean;
        private var _blind:String;
        private var _seat:int;
        private var _remove_next_turn:Boolean;
        private var _game_id:Boolean;

        private var _cards:Array=[]; /*of Cards*/
        public var  action:PlayerState;

        /*Player Stats*/
        private var _rank:int;
        private var _percentile:int;

        /*Player Chips*/
        private var _money:Number;
        public var bet:Number;

        private var _moneyInGame:Number;

        public function Player(PacketPokerPlayerArrive:Object)
        {
            setPlayer(PacketPokerPlayerArrive);
        }

        public function get name():String
        {
            return _name;
        }

        public function get seat():int
        {
            return _seat;
        }

        public function get money():int
        {
            return _money;
        }

        public function set cards(cards:Array):void
        {
            _cards = cards ;
        }

        public function get cards():Array
        {
            return _cards;
        }

        public function setPlayer(PacketPokerPlayerArrive:Object):void
        {
            _name = PacketPokerPlayerArrive.name;
            _url = PacketPokerPlayerArrive.url;
            _auto = PacketPokerPlayerArrive.auto;
            _outfit = PacketPokerPlayerArrive.outfit;
            _wait_for = PacketPokerPlayerArrive.wait_for;
            _buy_in_payed = PacketPokerPlayerArrive.buy_in_payed;
            _serial = PacketPokerPlayerArrive.serial;
            _sit_out_next_turn= PacketPokerPlayerArrive.sit_out_next_turn;
            _blind = PacketPokerPlayerArrive.blind;
            _seat = PacketPokerPlayerArrive.seat;
            _remove_next_turn = PacketPokerPlayerArrive.remove_next_turn;
            _game_id = PacketPokerPlayerArrive.game_id;
        }

        public function setChips(PacketPokerPlayerChips:Object):void
        {
             _money = PacketPokerPlayerChips.money;
        }

        public function setStats(PacketPokerPlayerStats:Object):void
        {
            _rank = PacketPokerPlayerStats.rank;
            _percentile =  PacketPokerPlayerStats.percentile;
        }
    }
}