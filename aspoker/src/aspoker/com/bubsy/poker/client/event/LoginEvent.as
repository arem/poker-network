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

package aspoker.com.bubsy.poker.client.event
{

import aspoker.com.bubsy.poker.client.model.PokerSession;
import aspoker.com.bubsy.poker.client.model.PokerUser;

import flash.events.Event;

public class LoginEvent extends Event
{
    public static const onPacketAuthOk:String = "PacketAuthOk";
    public static const onPacketSerial:String = "PacketSerial";
    public static const onPacketAuthRefused:String = "PacketAuthRefused";
    public var packet:Object;

    public function LoginEvent(type:String,data:Object=null,
        header:Array=null
        )
    {
        this.packet = data ;

        switch(type)
        {
            case "PacketAuthOk": break;
            case "PacketSerial":
            {
                PokerUser.UserSerial = data.serial;
                break;
            }
            default: trace(type);
        }
        super(type);
    }
}

}