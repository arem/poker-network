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
import aspoker.com.bubsy.poker.client.model.Session;

import com.adobe.serialization.json.JSON;
import com.bubzy.net.http.HTTPClient;
import com.bubzy.utils.Logger;

import flash.events.Event;
import flash.events.EventDispatcher;
import flash.events.HTTPStatusEvent;
import flash.net.URLRequest;

public class JsonStream extends EventDispatcher
{
    private var _restURL:String = "";
    private var _httpClient:HTTPClient = new HTTPClient;

    public function JsonStream():void
    {
        _httpClient = new HTTPClient();
        _httpClient.addEventListener("complete", onComplete);
        _httpClient.addEventListener("httpStatus", onHTTPStatus);
        _httpClient.addEventListener("close", handleError);
    }

    protected  function sendREST(packet:Object,session:Session=null):void
    {
      var request:URLRequest = new URLRequest();
      _restURL = Session.getUrl();
      request.url = _restURL;
      request.data=JSON.encode(packet);
      _httpClient.httpPOST(request);
    }

    protected function _dispatchEvent(pokerPacket:Object):void
    {
           Logger.log(pokerPacket.type);
    }

    private function onComplete(event:Event):void
    {
        var responseContent:String = HTTPClient(event.target).responseContent ;
        var results:Array = JSON.decode(responseContent);

        for (var i:int=0; i < results.length; i++)
        {
            if (results[i])
            {
                _dispatchEvent(results[i]);
            }
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
       // Logger.log("httpStatus " + event.status);
    }

    public function setRole():void
    {
        var packetPokerSeat:Object = {};
        packetPokerSeat.type = "PacketPokerSetRole",
        packetPokerSeat.roles = "PLAY";
        sendREST(packetPokerSeat);
    }

    public function plocale():void
    {
        var plocale:Object = {};
        plocale.serial = 6;
        plocale.locale = "en_US.UTF-8";
        sendREST(plocale);
    }
}
}