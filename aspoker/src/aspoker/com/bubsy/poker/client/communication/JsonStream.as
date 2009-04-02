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
import flash.utils.setTimeout;

import mx.effects.Pause;

public class JsonStream extends EventDispatcher
{
    private var _restURL:String = "";
    private var _httpClient:HTTPClient = new HTTPClient;

    public static const QUEUE_RUNNING:int=1;
    public static const QUEUE_WAIT:int=0;

    public var timeout:Number = 6000 ;
    private var lastRequestStartTime:Date;

    private var maxPoolSize:int=5;
    private var queue:Array = new Array();
    private var state:int = QUEUE_WAIT;


    public function JsonStream():void
    {
        _httpClient = new HTTPClient();
        _httpClient.addEventListener("complete", onComplete);
        _httpClient.addEventListener("httpStatus", onHTTPStatus);
        _httpClient.addEventListener("close", handleError);
    }

    private function sendNext():void
    {
        if (state == QUEUE_WAIT && queue.length>0)
        {
            var packet:Object = queue.pop();
            var request:URLRequest = new URLRequest();

            state = QUEUE_RUNNING;
            lastRequestStartTime = new Date();
            _restURL = Session.getUrl();
            request.url = _restURL;
            request.data=JSON.encode(packet);
            _httpClient.cookie = Session.cookie;
            _httpClient.useCookie = true;
            _httpClient.httpPOST(request);
        }
    }

    private function checkTimeOut():Boolean
    {
         if (state == QUEUE_WAIT)
         {
             return false;
         }

        var currentTime:Date = new Date();
        var delta:Number = currentTime.getTime() - lastRequestStartTime.getTime();

        if ( delta > timeout && state == QUEUE_RUNNING)
        {
            trace ("timeout :" + delta);
            return true;
        }
        return false;
    }

    protected  function sendREST(packet:Object):void
    {
        checkTimeOut();

        if (queue.length<=maxPoolSize)
        {
            queue.push(packet);
        } else {
            throw new Error("Pool size is too big");
        }

        if (state == QUEUE_WAIT)
        {
            sendNext();
        }
    }

    protected function _dispatchEvent(pokerPacket:Object):void
    {
           Logger.log(pokerPacket.type);
    }

    private function onComplete(event:Event):void
    {
        var responseContent:String = HTTPClient(event.target).responseContent ;
        var results:Array = JSON.decode(responseContent);

        Session.cookie = HTTPClient(event.target).cookie;

        for (var i:int=0; i < results.length; i++)
        {
            if (results[i])
            {
                //trace(results[i].type);
                _dispatchEvent(results[i]);
            }
        }
        state = QUEUE_WAIT;
        setTimeout(sendNext,1);
    }

    private function my_delayedFunction():void
    {

    }

    private function handleError():void
    {
         trace("soket closed");
    }

    private function httpStatusHandler(event:Event):void
    {
        Logger.log(event.target.data);
    }

    private function onHTTPStatus(event:HTTPStatusEvent):void
    {
        //trace("httpStatus " + event.status);
    }

    public function setRole():void
    {
        var packetPokerSeat:Object = {};
        packetPokerSeat.type = "PacketPokerSetRole",
        packetPokerSeat.roles = "PLAY";
        packetPokerSeat.serial = 4;
        sendREST(packetPokerSeat);
    }

    public function plocale():void
    {
        var plocale:Object = {};
        plocale.type = "PacketPokerSetLocale";
        plocale.serial = 4;
        plocale.locale = "en";
        plocale.game_id = 1;
        sendREST(plocale);
    }
}
}