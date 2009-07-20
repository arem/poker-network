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
import aspoker.com.bubsy.poker.client.event.TableListEvent;

import aspoker.com.bubsy.poker.client.model.Table;
import aspoker.com.bubsy.poker.client.model.Lobby;
import aspoker.com.bubsy.poker.client.model.User;

import com.adobe.serialization.json.JSON;

public class JsonStreamProxy extends JsonStream
{

    public static var tablelist:Array = [] /* of Table*/;
    public static var lobby:Lobby;
    public static var user:User;

    public static function setLobby(lobby:Lobby):void
    {
        JsonStreamProxy.lobby = lobby;
    }
    
    public static function setUser(user:User):void
    {
        JsonStreamProxy.user = user ;
    }

    public static function register(table:Table):void
    {
        tablelist[table.gameId] = table;
    }

    public static function unregister(table:Table):void
    {
        tablelist[table.gameId] = null;
    }

    public function JsonStreamProxy()
    {
        super();
    }

    override protected function _dispatchEvent(pokerPacket:Object):void
    {
        var gameid:int = pokerPacket.game_id;

        if (gameid == 0) { 
             gameid = pokerPacket.id;
        }

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
                tablelist[gameid]._onPacketPokerTable(pokerPacket);
                break;
            }

            case TableEvent.onPacketPokerPlayerArrive:
            {
                tablelist[gameid]._onPacketPokerPlayerArrive(pokerPacket);
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
                tablelist[gameid]._onPokerPosition(pokerPacket);
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
                tablelist[gameid]._onPlayerChips(pokerPacket);
                break;
            }

            case TableEvent.onPacketPokerBuyInLimits:
            {
                tablelist[gameid]._onBuyInLimits(pokerPacket);
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

            /* PacketPokerSeats was deprecated when the seat number was added to
              the PlayerArrive and PlayerLeave packets */

            /* case TableEvent.onPacketPokerSeats:
            {
                tablelist[gameid]._onPacketPokerSeats(pokerPacket);
                break;
            }
            */
            
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
                tablelist[gameid]._onSitOut(pokerPacket);
                break;
            }

            case TableEvent.onPacketPokerPlayerLeave:
            {
                tablelist[gameid]._onPacketPokerPlayerLeave(pokerPacket);
                break;
            }

            case TableEvent.onPacketPokerInGame:
            {
                tablelist[gameid]._onPokerInGame(pokerPacket);
                break;
            }

            case TableEvent.onPacketPokerSit:
            {
                tablelist[gameid]._onSit(pokerPacket);
                break;
            }

            case TableEvent.onPacketPokerCheck:
            {
                tablelist[gameid]._onPacketPokerCheck(pokerPacket);
                break;
            }

            case TableEvent.onPacketPokerCall:
            {
                tablelist[gameid]._onPacketPokerCall(pokerPacket);
                break;
            }

            case TableEvent.onPacketPokerFold:
            {
                tablelist[gameid]._onPacketPokerFold(pokerPacket);
                break;
            }

            case TableEvent.onPacketPokerRaise:
            {
                tablelist[gameid]._onPacketPokerRaise(pokerPacket);
                break;
            }

            case TableEvent.onPacketPokerBlindRequest:
            {
                tablelist[gameid]._onPacketPokerBlindRequest(pokerPacket);
                break;
            }

            case TableEvent.onPacketPokerBlind:
            {
                tablelist[gameid]._onPacketPokerBlind(pokerPacket);
                break;
            }

            case TableEvent.onPacketPokerPlayerCards:
            {
                tablelist[gameid]._onPacketPokerPlayerCards(pokerPacket);
                break;
            }

            case TableEvent.onPacketPokerStart:
            {
                tablelist[gameid]._onPacketPokerStart(pokerPacket);
                break;
            }

            case TableEvent.PacketPokerBoardCards:
            {
                tablelist[gameid]._PacketPokerBoardCards(pokerPacket);
                break;
            }

            case TableEvent.onPacketPokerState:
            {
                tablelist[gameid]._onPacketPokerState(pokerPacket);
                break;
            }

            case TableEvent.onPacketPokerDealer:
            {
                tablelist[gameid]._onPacketPokerDealer(pokerPacket);
                break;
            }

            case TableEvent.onPacketPokerPlayerCards:
            {
                tablelist[gameid]._onPacketPokerPlayerCards(pokerPacket);
                break;
            }

            case "PacketPokerTableList":
            {
                lobby.onPacketPokerTableList(pokerPacket);
                break;
            }
            
            case "PacketAuthRefused":
            {
                user.onPacketAuthRefused (
                    new LoginEvent(
                        LoginEvent.onPacketAuthRefused,
                        -1,
                        pokerPacket.message
                        )
                );
                break;
            }

            case "PacketAuthOk":
            {
                user.onPacketAuthOK(
                    new LoginEvent(
                        LoginEvent.onPacketAuthOk,
                        pokerPacket.serial
                        )
                );
                break;
            }

            case "PacketSerial":
            {
                user.onPacketSerial(
                    new LoginEvent(
                        LoginEvent.onPacketSerial,
                        pokerPacket.serial
                    )
                );
                break;
            }

            case "PacketAuthRefused":
            {
               user.onPacketAuthRefused(
                    new LoginEvent
                    (
                        LoginEvent.onPacketAuthRefused
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

    public function getTables():void
    {
        var packetPokerTableSelect:Object = {};
        packetPokerTableSelect.type = "PacketPokerTableSelect";
        packetPokerTableSelect.string = "";
        sendREST(packetPokerTableSelect);
    }

    public function loggin(userName:String,userPassword:String):void
    {
        var packetLogin:Object = {};
        packetLogin.password = userPassword;
        packetLogin.name = userName;
        packetLogin.type = "PacketLogin";
        sendREST(packetLogin);
    }

    public function logout():void
    {
        var packetPokerGetPersonalInfo:Object = {};
        packetPokerGetPersonalInfo.type = "PacketLogout";
        sendREST(packetPokerGetPersonalInfo);
    }

    public function personalInfo(userSerial:int):void
    {
        var packetPokerGetPersonalInfo:Object = {};
        packetPokerGetPersonalInfo.type = "PacketPokerGetPersonalInfo";
        packetPokerGetPersonalInfo.serial = userSerial;
        sendREST(packetPokerGetPersonalInfo);
    }

}
}
