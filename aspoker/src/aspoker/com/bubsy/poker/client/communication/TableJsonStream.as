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
import aspoker.com.bubsy.poker.client.event.LoginEvent;
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
            case TableEvent.onPacketPokerPosition:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerPosition,
                    pokerPacket
                    )
                );
                break;
            }

            case LoginEvent.onPacketAuthRequest:
            {
                dispatchEvent(
                    new LoginEvent(
                        LoginEvent.onPacketAuthRequest
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

            case TableEvent.onPacketPokerSitOut:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerSitOut,
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

            case TableEvent.onPacketPokerInGame:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerInGame,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerSit:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerSit,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerCheck:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerCheck,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerCall:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerCall,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerFold:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerFold,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerRaise:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerRaise,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerBlindRequest:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerBlindRequest,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerBlind:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerBlind,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerPlayerCards:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerPlayerCards,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerStart:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerStart,
                    pokerPacket
                    )
                );
                break;
            }


            case TableEvent.PacketPokerBoardCards:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.PacketPokerBoardCards,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerState:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerState,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerDealer:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerDealer,
                    pokerPacket
                    )
                );
                break;
            }

            case TableEvent.onPacketPokerPlayerCards:
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerPlayerCards,
                    pokerPacket
                    )
                );
                break;
            }

            default:
            {
               // trace("unknown packet:" + pokerPacket.type);
                trace(JSON.encode(pokerPacket));
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
        packetPokerSit.game_id = gameid;
        packetPokerSit.serial = userSerial;
        sendREST(packetPokerSit);
    }

    public function fold(gameid:int,userSerial:int):void
    {
        var packetPokerFold:Object = {};
        packetPokerFold.type = "PacketPokerFold",
        packetPokerFold.game_id = gameid;
        packetPokerFold.serial = userSerial;
        sendREST(packetPokerFold);
    }

    public function call(gameid:int,userSerial:int):void
    {
        var packetPokerCall:Object = {};
        packetPokerCall.type = "PacketPokerCall",
        packetPokerCall.game_id = gameid;
        packetPokerCall.serial = userSerial;
        sendREST(packetPokerCall);
    }

    public function raise(gameid:int,userSerial:int,amount:int):void
    {
        var packetPokerRaise:Object = {};
        packetPokerRaise.type = "PacketPokerRaise",
        packetPokerRaise.game_id = gameid;
        packetPokerRaise.serial = userSerial;
        packetPokerRaise.amount = amount;
        sendREST(packetPokerRaise);
    }

    public function check(gameid:int,userSerial:int):void
    {
        var PacketPokerCheck:Object = {};
        PacketPokerCheck.type = "PacketPokerCheck",
        PacketPokerCheck.game_id = gameid;
        PacketPokerCheck.serial = userSerial;
        sendREST(PacketPokerCheck);
    }

    public function blind(gameid:int,userSerial:int):void
    {
        var PacketPokerBlind:Object = {};
        PacketPokerBlind.type = "PacketPokerBlind",
        PacketPokerBlind.game_id = gameid;
        PacketPokerBlind.serial = userSerial;
        sendREST(PacketPokerBlind);
    }

    public function sitOut(gameid:int,userSerial:int):void
    {
        var packetPokerSitOut:Object = {};
        packetPokerSitOut.type = "PacketPokerSitOut",
        packetPokerSitOut.game_id = gameid;
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

    public function buyIn(gameid:int,userSerial:int,amount:Number):void
    {
        var packetPokerBuyIn:Object = {};
        packetPokerBuyIn.type = "PacketPokerBuyIn";
        packetPokerBuyIn.amount = amount;
        packetPokerBuyIn.game_id = gameid;
        packetPokerBuyIn.serial = userSerial;
        sendREST(packetPokerBuyIn);
    }
        public function autoBuildAnt(gameid:int,userSerial:int):void
    {
        var PacketPokerAutoBlindAnte:Object = {};
        PacketPokerAutoBlindAnte.type = "PacketPokerAutoBlindAnte";
        PacketPokerAutoBlindAnte.serial = userSerial;
        PacketPokerAutoBlindAnte.game_id = gameid;
        sendREST(PacketPokerAutoBlindAnte);
    }
}
}