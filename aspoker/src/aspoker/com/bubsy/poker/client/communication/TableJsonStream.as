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
import aspoker.com.bubsy.poker.client.event.TableEvent;

import com.adobe.serialization.json.JSON;

public class TableJsonStream extends JsonStream
{

    public function TableJsonStream()
    {
        super();
    }

    override protected function _dispatchEvent(pokerPacket:Object):void
    {
        switch(pokerPacket.type)
        {
            case TableEvent.onPacketPokerSeat:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerSeat,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerTable:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerTable,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerPlayerArrive:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerPlayerArrive,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerError:
            {
                trace("erreur" + JSON.encode(pokerPacket));
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerError,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerPlayerStats:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerPlayerStats,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerPlayerChips:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerPlayerChips,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerBuyInLimits:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerBuyInLimits,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerUserInfo:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerUserInfo,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerBatchMode:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerBatchMode,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerSeats:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerSeats,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerStreamMode:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerStreamMode,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerPlayerLeave:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerPlayerLeave,
                    pokerPacket
                    )
                );
                break;
            }

            default:
            {
                trace("unknown packet:" + pokerPacket.type);
            }
        }
    }

    public function tableJoin(gameid:int,userSerial:int):void
    {
        var packetPokerTableJoin:Object = {};
        packetPokerTableJoin.type = "PacketPokerTableJoin";
        packetPokerTableJoin.game_id = gameid;
      //  packetPokerTableJoin.serial = userSerial;
        sendREST(packetPokerTableJoin);
    }

    public function sit(gameid:int,userSerial:int):void
    {
        var packetPokerSit:Object = {};
        packetPokerSit.type = "PacketPokerSit",
        packetPokerSit.game_id =gameid;
        packetPokerSit.serial = userSerial;
        sendREST(packetPokerSit);
    }

    public function sitOut(gameid:int,userSerial:int):void
    {
        var packetPokerSitOut:Object = {};
        packetPokerSitOut.type = "PacketPokerSitOut",
        packetPokerSitOut.game_id =gameid;
        packetPokerSitOut.serial = userSerial;
        sendREST(packetPokerSitOut);
    }

    public function Poll(gameid:int):void
    {
        var packetPokerSeat:Object = {};
        packetPokerSeat.type = "PacketPokerPoll",
        packetPokerSeat.game_id =gameid;
        sendREST(packetPokerSeat);
    }

    public function seat(gameid:int,userSerial:int,seat:int):void
    {
        var packetPokerSeat:Object = {};

        packetPokerSeat.type = "PacketPokerSeat",
        packetPokerSeat.serial = userSerial;
        packetPokerSeat.game_id =gameid;
        packetPokerSeat.seat = seat;
        sendREST(packetPokerSeat);
    }

    public function quit(gameid:int):void
    {
        var packetPacketPokerTableQuit:Object = {};
        packetPacketPokerTableQuit.type = "PacketPokerTableQuit",
        packetPacketPokerTableQuit.game_id =gameid;
        sendREST(packetPacketPokerTableQuit);
    }

    public function buyIn(gameid:int,userSerial:int):void
    {
        var packetPokerBuyIn:Object = {};
        packetPokerBuyIn.type = "PacketPokerBuyIn";
        packetPokerBuyIn.amount = 200000;
        packetPokerBuyIn.game_id = gameid;
        packetPokerBuyIn.serial = userSerial;
        sendREST(packetPokerBuyIn);
    }
}
}