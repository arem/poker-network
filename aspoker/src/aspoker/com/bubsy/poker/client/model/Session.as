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

package aspoker.com.bubsy.poker.client.model
{

import mx.messaging.channels.StreamingAMFChannel;

public class Session
{
    private static var _sessionCount:int = 0;
    private static var _twistedSession:String="";
    private static var _cookie:Array/*of cookieItem*/=[];

    private static var restUrl:String = "http://127.0.0.1:19382/REST";
    public static var UserSerial:int;

    public static function set cookie(cookie:Array):void
    {
        if (_cookie.length<=0 && cookie.length>0)
        {
            _cookie = cookie;
        }
    }

    public static function flush():void
    {
          _cookie = [];
    }

    public static function get cookie():Array
    {
        return _cookie;
    }

    public static function getUrl():String
    {
        if (getTwistedSessionFromCookie() == "") {
            return restUrl + "?session=yes&count=" + incrementSessionCount();
        } else {
            return restUrl + "?session=yes" + "&count=" + incrementSessionCount();
            //return restUrl + "?name=" + getTwistedSessionFromCookie() + "&count=" + incrementSessionCount();
        }
    }

    public static function getTwistedSession():String
    {
        return _twistedSession;
    }

    public static function getSessionCountFromCookie():int
    {
        return _sessionCount;
    }

    private static function getTwistedSessionFromCookie():String
    {
        var p:Array = _twistedSession.split(";");

        for each(var s:String in p)
        {
            var c:Array = s.split("=");
            if (c[0]=="TWISTED_SESSION")
                return c[1];
        }
        return "";
    }

    private static function incrementSessionCount():int
    {
        _sessionCount++;
        return _sessionCount;
    }
}

}