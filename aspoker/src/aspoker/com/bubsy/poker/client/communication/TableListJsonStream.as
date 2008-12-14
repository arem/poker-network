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

package aspoker.com.bubsy.poker.client.communication
{

import com.bubzy.utils.Logger;
import aspoker.com.bubsy.poker.client.event.TableListEvent;
import aspoker.com.bubsy.poker.client.model.Session;
import com.adobe.serialization.json.JSON;

public class TableListJsonStream extends JsonStream
{
	public function TableListJsonStream()
	{
		super();
	}

	override protected function _dispatchEvent(pokerPacket:Object):void
    {
   		Logger.log(pokerPacket.type);
		trace(JSON.encode(pokerPacket));
        switch(pokerPacket.type)
        {
            case "PacketPokerTableList":
            {
                dispatchEvent(
                    new TableListEvent(
                    TableListEvent.onPacketPokerTableList,
                    pokerPacket
                    )
                );
                break;
            }
            default: trace(JSON.encode(pokerPacket));
         }
	}

	public function getTables():void
    {
        var packetPokerTableSelect:Object = {};
        packetPokerTableSelect.type = "PacketPokerTableSelect";
        packetPokerTableSelect.string = "";
        sendREST(packetPokerTableSelect);
    }
}
}