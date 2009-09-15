////////////////////////////////////////////////////////////////////////////////
//
//     Copyright (C) 2008 Bruno Garnier <bruno.garnier@gmail.com>
//
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     This program is distaEvent
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
import org.httpclient.HttpClient;
import org.httpclient.HttpRequest;
import org.httpclient.HttpResponse;
import org.httpclient.events.*;
import com.adobe.net.URI;
import flash.utils.ByteArray;

import flash.events.Event;
import flash.events.EventDispatcher;
import flash.events.HTTPStatusEvent;
import flash.net.URLRequest;
import flash.utils.setTimeout;

public class JsonStream extends EventDispatcher
{
    public static const QUEUE_RUNNING:int = 1;
    public static const QUEUE_WAIT:int = 0;
    public var timeout:Number = 6000 ;
    
    private var _lastRequestStartTime:Date;
    private var _restURL:String = "";
    private var _httpClient:HttpClient = new HttpClient;
    private var _maxPoolSize:int = 5;
    private var _queue:Array = new Array();
    private var _state:int = QUEUE_WAIT;
    private var _response:String;

    public function JsonStream():void
    {
        _httpClient.addEventListener(HttpResponseEvent.COMPLETE, _onComplete);
        _httpClient.addEventListener(HttpDataEvent.DATA, _onData);
    }

    private function _sendNext():void
    {
        if (_state == QUEUE_WAIT && _queue.length>0)
        {
            var packet:String = _queue.pop();
            var request:URLRequest = new URLRequest();

            _state = QUEUE_RUNNING;
            _lastRequestStartTime = new Date();
            _post(packet);
        }
    }

    private function _post(packet:String):void
    {
        _response = "";
        var uri:URI = new URI(Session.getUrl());
        var jsonData:ByteArray = new ByteArray();
        var contentType:String = "application/json";

        jsonData.writeUTFBytes(packet);
        jsonData.position = 0;
        _httpClient.post(uri, jsonData, contentType);
    }

    private function _checkTimeOut():Boolean
    {
         if (_state == QUEUE_WAIT)
         {
             return false;
         }

        var currentTime:Date = new Date();
        var delta:Number = currentTime.getTime() - _lastRequestStartTime.getTime();

        if (delta > timeout && _state == QUEUE_RUNNING)
        {
            trace ("timeout :" + delta);
            return true;
        }
        return false;
    }

    protected  function sendREST(packet:Object):void
    {
        _checkTimeOut();
        
        var str:String = JSON.encode(packet);
        var searchIndex:int = _queue.indexOf(str);
        
        if (searchIndex > -1) 
        {
            throw new Error("flood");
            return false;
        }

        if (_queue.length <= _maxPoolSize)
        {
            _queue.push(str);
        } else {
            throw new Error("Pool size is too big");
            return false;
        }

        if (_state == QUEUE_WAIT)
        {
            _sendNext();
        }
    }

    protected function _dispatchEvent(pokerPacket:Object):void
    {
    }

    private function _onComplete(event:HttpResponseEvent):void
    {
        var results:Array = JSON.decode(_response);

        for (var i:int=0; i < results.length; i++)
        {
            if (results[i])
            {
                _dispatchEvent(results[i]);
            }
        }
        _state = QUEUE_WAIT;
        setTimeout(_sendNext,1);
    }

    private function _onData(event:HttpDataEvent):void
    {
        _response += event.readUTFBytes();
    }

    public function ping():void
    {
       var packetPokerPing:Object = {};
       packetPokerPing.type = "PacketPing";
       sendREST(packetPokerPing); 
    }    
}
}
