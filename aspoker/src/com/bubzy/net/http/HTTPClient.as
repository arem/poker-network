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

package com.bubzy.net.http
{

import flash.events.*;
import flash.net.*;
import flash.system.Security;
import flash.utils.*;

public class HTTPClient extends EventDispatcher
{
    public const HTTP_END_LINE:String = "\r\n" ;
    public const HTTP_END_HEADER:String = "\r\n\r\n" ;
    public const HTTP_VERSION:String = "HTTP/1.0" ;

    public  var useCookie:Boolean = true;

    private var _httpRequest:URLRequest;
    private var _httpResponseHeaders:Array /* of String */ = [];
    private var _httpResponseContent:String = "";
    private var _httpResponseBuffer:String = "";
    private var _httpResponseStatusText:String = "";
    private var _httpResponseCookies:Array=[]/* of CookieItem */;
    private var _httpResponseVersion:Number;
    private var _httpResponseStatusCode:int;

    private var _httpRequestPath:String;
    private var _httpRequestServer:String;

    private var _httpPort:uint = 80;
    private var _httpSocket:Socket;

    private var _bytesLoaded:int = 0;
    private var _bytesTotal:int = 0;

    public function HTTPClient()
    {
        _httpSocket = new Socket();
        _httpSocket.addEventListener("connect" , _onConnectEvent);
        _httpSocket.addEventListener("close" , _onCloseEvent);
        _httpSocket.addEventListener("ioError" , _onIOErrorEvent);
        _httpSocket.addEventListener("securityError" ,_onSecurityErrorEvent);
        _httpSocket.addEventListener("socketData" , _onSocketDataEvent);
    }

    public function httpPOST(request:URLRequest):void
    {
        _httpRequest = request;
        _httpRequest.method = URLRequestMethod.POST;
        _init();
        _connect();
    }

    public function httpGET(request:URLRequest):void
    {
         _httpRequest = request;
         _httpRequest.method = URLRequestMethod.GET;
         _init();
         _connect();
    }

    public function close():void
    {
       if(_httpSocket.connected)
        {
            _httpSocket.close();
            dispatchEvent(new Event(Event.CLOSE));
        }
    }

    public function get responseHeaders():Array
    {
        return _httpResponseHeaders;
    }

    public function get responseContent():String
    {
        return _httpResponseContent;
    }

    public function get bytesLoaded():int
    {
        return _bytesLoaded;
    }

    public function get bytesTotal ():int
    {
        return _bytesTotal;
    }

    private function _connect():Boolean
    {
        Security.allowDomain("*");
        Security.loadPolicyFile("xmlsocket://"
            + _httpRequestServer + ":843");

        _httpSocket.connect(_httpRequestServer, _httpPort);
        return true;
    }

    private function _init():void
    {
        _initResponse();
        _parseURL();
        _initResquestHeaders();
    }

    private function _initResponse():void
    {
        _bytesLoaded = 0;
        _bytesTotal = 0;
        _httpResponseHeaders = [];
        _httpResponseContent = "";
        _httpResponseBuffer = "";
        _httpResponseVersion = 0
        _httpResponseStatusCode = 0
        _httpResponseStatusText = "";
    }

    private function _onConnectEvent(event:Event):void
    {
        _sendRequest();
        dispatchEvent(new Event(Event.CONNECT));
    }

    private function _onCloseEvent(event:Event):void
    {
        _parseResponse();
        dispatchEvent(new Event(Event.COMPLETE));
    }

    private function _onIOErrorEvent(event:IOErrorEvent):void
    {
       trace("IOErrorEvent");
        dispatchEvent(event);
    }

    private function _onSecurityErrorEvent(event:SecurityErrorEvent):void
    {
        trace("SecurityErrorEvent");
        dispatchEvent(event.clone());
    }

    private function _setCookie(str:String):Array
    {
        var p:Array = str.split(";");
        _httpResponseCookies = [];

        for each(var s:String in p)
        {
            var c:Array = s.split("=");
            var cookieItem:CookieItem = new CookieItem(c[0],c[1]);
            _httpResponseCookies.push(cookieItem);
        }
        return _httpResponseCookies;
    }

    public function get cookie():Array
    {
        return _httpResponseCookies;
    }

    public function set cookie(cookie:Array/* of CookieItem */):void
     {
            _httpResponseCookies = cookie;
    }

    private function cookieToString():String
    {
        var cookieString:String = "";

        if (_httpResponseCookies.length<=0)
        {
            return cookieString;
        }

        for each(var p:CookieItem in _httpResponseCookies)
        {
            cookieString+= p.name
                + "="
                + p.value
                + ";" ;
        }

        return cookieString;
    }

    private function _parseResponseHeader(str:String):Boolean
    {
        var pattern1:RegExp = new RegExp("^([a-zA-Z0-9._%+-]+): (.*)");
        var results:Array = str.match(pattern1);

        if (results)
        {
            if ( results[1] == "Set-Cookie" )
            {
                _setCookie(results[2]);
            } else {
                _httpResponseCookies = [];
            }

            _httpResponseHeaders[results[1]] = results[2];
            return true;
        }
        return false;
    }

    private function _parseResponseHeaderStatus(str:String):Boolean
    {
        var pattern1:RegExp = new RegExp(
            "^HTTP\/(?P<version>1.[0-9]) "
            + "(?P<statusCode>[1-5][01][0-9]) "
            + "(?P<statusText>[a-zA-Z0-9._%+-]+)"
            );

        var results:Array = str.match(pattern1);

        if (results)
        {
            _httpResponseVersion = results.version ;
            _httpResponseStatusCode = results.statusCode;
            _httpResponseStatusText = results.statusText;
            dispatchEvent(new HTTPStatusEvent(HTTPStatusEvent.HTTP_STATUS,
                false, false, _httpResponseStatusCode));

            return true;
        }
        return false;
    }

    private function _parseURL():Boolean
    {
        var testp:String = _httpRequest.url ;
        var reg:RegExp = /(?P<protocol>[a-zA-Z]+) : \/\/  (?P<host>[^:\/]*) (:(?P<port>\d+))?  ((?P<path>[^?]*))? ((?P<parameters>.*))? /x;
        var results:Array = reg.exec(testp);

        _httpRequestServer = results.host;
        _httpRequestPath = results.path + results.parameters;

        if (results.port!="")
           _httpPort = results.port;

        return true;
    }

    private function _parseResponse():void
    {
        var response:Array = _httpResponseBuffer.split(HTTP_END_HEADER);
        var headerArr:Array = response[0].split(HTTP_END_LINE);

        _httpResponseContent += response[1];

        for each(var p:String in headerArr)
        {
            _parseResponseHeaderStatus(p);
            _parseResponseHeader(p);
        }

        if(_httpResponseHeaders["Content-Length"])
        {
            _bytesTotal = int(_httpResponseBuffer["Content-Length"])
                + _httpResponseBuffer.length;
        }
    }

    private function _onSocketDataEvent(event:ProgressEvent):void
    {
        var buffer:String = "";

        _bytesLoaded += _httpSocket.bytesAvailable;
        dispatchEvent(new ProgressEvent(ProgressEvent.PROGRESS, false,
            false, _bytesLoaded, _bytesTotal)
        );

        buffer = _httpSocket.readUTFBytes(_httpSocket.bytesAvailable);
        _httpResponseBuffer += buffer;
    }

    private function _initResquestHeaders():void
    {
        _httpRequest.requestHeaders.push(
            new URLRequestHeader("Host",_httpRequestServer)
         );

        _httpRequest.requestHeaders.push(
            new URLRequestHeader("Content-Length",_httpRequest.data.length)
         );

        if (_httpResponseCookies && useCookie)
        {
            _httpRequest.requestHeaders.push(
                new URLRequestHeader("Cookie",cookieToString()));
        }
    }

    private function _sendRequest():void
    {
        var requestHeaders:Array = _httpRequest.requestHeaders;
        var HttpRequest:String = _httpRequest.method
            + " " + _httpRequestPath + " "
            + HTTP_VERSION + HTTP_END_LINE;

        if(requestHeaders.length > 0)
        {
            for each(var httpHeader:URLRequestHeader in requestHeaders)
            {
                HttpRequest+= httpHeader.name + ":"
                    + httpHeader.value + HTTP_END_LINE;
            }
        }

        _httpSocket.writeUTFBytes(HttpRequest + HTTP_END_LINE);

        if (_httpRequest.method == URLRequestMethod.POST)
            _httpSocket.writeUTFBytes(_httpRequest.data  + HTTP_END_LINE);

        _httpSocket.flush();
    }
}

}