package aspoker.com.bubsy.poker.client.communication
{

import aspoker.com.bubsy.poker.event.LoginEvent;
import aspoker.com.bubsy.poker.model.PokerSession;

import com.adobe.serialization.json.JSON;
import com.bubzy.net.http.HTTPClient;
import com.bubzy.utils.Logger;

import flash.events.Event;
import flash.events.EventDispatcher;
import flash.events.HTTPStatusEvent;
import flash.net.URLRequest;

public class PokerConnection extends EventDispatcher
{
    private var _restURL:String = "";
    private var _httpClient:HTTPClient = new HTTPClient;

    public function PokerConnection():void
    {
        _httpClient = new HTTPClient();
        _httpClient.addEventListener("complete", onComplete);
        _httpClient.addEventListener("httpStatus", onHTTPStatus);
        _httpClient.addEventListener("close", handleError);
    }

    private  function sendREST(packet:Object,session:PokerSession=null):void
    {
      var request:URLRequest = new URLRequest();
      _restURL = PokerSession.getUrl();
      request.url = _restURL;
      request.data=JSON.encode(packet);
      _httpClient.httpPOST(request);
    }

   private function _dispatchEvent(pokerPacket:Object):void
   {
        switch(pokerPacket.type)
        {
            case "PacketAuthOk":
            {    dispatchEvent
                    (new LoginEvent(LoginEvent.onPacketAuthOk
                        ,pokerPacket));
                break;
            }
            case "PacketSerial":
            {
            //  PokerSession.UserSerial = results[i].serial;
              PokerSession.setCookie(pokerPacket.cookie);

                dispatchEvent(
                    new LoginEvent(LoginEvent.onPacketSerial
                        ,pokerPacket
                    )
                );
                break;
            }
            case "PacketAuthRefused":
            {
               dispatchEvent(
                    new LoginEvent(
                    LoginEvent.onPacketAuthRefused
                    )
                );
                break;
            }
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
            case "PacketAuthRequest" :
            case "PacketPokerTable":
            case "PacketPokerBuyInLimits":
            case "PacketPokerBatchMode":
            case "PacketPokerSeats":
            case "PacketPokerStreamMode":
            case "PacketPokerError":
            case "PacketPokerPlayerInfo":
            case "PacketAuthRefused":
            default:
            {
               Logger.log(pokerPacket.type);
            }
         }
   }

    private function onComplete(event:Event):void
    {
        var responseContent:String = HTTPClient(event.target).responseContent ;
        var results:Array = JSON.decode(responseContent);
        //Logger.log(responseContent);

        for (var i:int=0; i < results.length; i++)
        {
            if (results[i])
                _dispatchEvent(results[i]);
        }
    }

    private function handleError():void
    {
         Logger.log("soket closed");
    }

    private function httpStatusHandler(event:Event):void
    {
        Logger.log(event.target.data);
    }

    private function onHTTPStatus(event:HTTPStatusEvent):void
    {
        Logger.log("httpStatus " + event.status);
    }

    public function seat():void
    {
        var packetPokerSeat:Object = {};
        packetPokerSeat.type = "PacketPokerSeat";
        packetPokerSeat.seat = 1;
        packetPokerSeat.game_id = 1;
        packetPokerSeat.serial = 6;
        sendREST(packetPokerSeat);
    }

    public function setRole():void
    {
        var packetPokerSeat:Object = {};
        packetPokerSeat.type = "PacketPokerSetRole",
        packetPokerSeat.roles = "PLAY";
        sendREST(packetPokerSeat);
    }

    public function getTables():void
    {
        var packetPokerTableSelect:Object = {};
        packetPokerTableSelect.type = "PacketPokerTableSelect";
        packetPokerTableSelect.string = "";
        sendREST(packetPokerTableSelect);
    }

    public function plocale():void
    {
        var plocale:Object = {};
        plocale.serial = 6;
        plocale.locale = "en_US.UTF-8";
        sendREST(plocale);
    }

    public function getPersonalInfo():void
    {
        var packetPokerGetPersonalInfo:Object = {};
        packetPokerGetPersonalInfo.type = "PacketPokerGetPlayerInfo";
       //packetPokerGetPersonalInfo.serial = 6;
        sendREST(packetPokerGetPersonalInfo);
     }

    public function getCashier():void
    {
        var packetPokerGetPersonalInfo:Object = {};
        packetPokerGetPersonalInfo.type = "PacketPokerGetUserInfo";
        packetPokerGetPersonalInfo.serial = 6;
        sendREST(packetPokerGetPersonalInfo);
    }

    public function tableJoin(gameid:int,userSerial:int):void
    {
        var packetPokerTableJoin:Object = {};
        packetPokerTableJoin.type = "PacketPokerTableJoin";
        packetPokerTableJoin.game_id = gameid;
        trace('userSerial'+userSerial);
        packetPokerTableJoin.serial = 6;
        sendREST(packetPokerTableJoin);
    }

    public function sit():void
    {
        var packetPokerSit:Object = {};
        packetPokerSit.type = "PacketPokerSit",
        packetPokerSit.game_id =1,
        packetPokerSit.serial = 6
        sendREST(packetPokerSit);
    }

    public function buyIn():void
    {
        var packetPokerBuyIn:Object = {};
        packetPokerBuyIn.type = "PacketPokerBuyIn";
        packetPokerBuyIn.amount = 200000;
        packetPokerBuyIn.game_id = 1;
        packetPokerBuyIn.serial = 6;
        sendREST(packetPokerBuyIn);
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
}
}