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
///////////////////////////////////////////////////////////////////////////////

package aspoker.com.bubsy.poker.client.event
{
    import flash.events.Event;

    public class TableEvent extends Event
    {
        public static const onPacketPokerTable:String = "PacketPokerTable";
        public static const onPacketPokerBuyInLimits:String = "PacketPokerBuyInLimits";
        public static const onPacketPokerBatchMode:String = "PacketPokerBatchMode";
        public static const onPacketPokerSeats:String = "PacketPokerSeats";
        public static const onPacketPokerStreamMode:String = "PacketPokerStreamMode";
        public static const onPacketPokerPlayerArrive:String = "PacketPokerPlayerArrive";
        public static const onPacketPokerPlayerStats:String = "PacketPokerPlayerStats";
        public static const onPacketPokerPlayerChips:String = "PacketPokerPlayerChips";
        public static const onPacketPokerClientPlayerChips:String = "PacketPokerClientPlayerChips";
        public static const onPacketPokerSeat:String = "PacketPokerSeat";
        public static const onPacketPokerSitOut:String = "PacketPokerSitOut";
        public static const onPacketPokerError:String = "PacketPokerError";
        public static const onPacketPokerUserInfo:String = "PacketPokerUserInfo"
        public static const onPacketPokerPlayerLeave:String = "PacketPokerPlayerLeave"

        public var packet:Object;

        public function TableEvent(type:String,packet:Object)
        {
            this.packet = packet;
            super(type);
        }

    }
}