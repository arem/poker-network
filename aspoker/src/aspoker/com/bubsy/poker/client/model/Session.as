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
    import aspoker.com.bubsy.poker.client.PokerClient;
    import flash.net.SharedObject;
    import com.adobe.utils.IntUtil;
    import com.adobe.crypto.SHA1;
    import mx.events.BrowserChangeEvent;
    import mx.utils.URLUtil;


public class Session
{
    private static var _auth_hash:String = "";
    private static var _session_uid:String = "";
    private static var _auth_cookie:String = "";
    private static var _localeSession:SharedObject;
    private static var _sessionCount:int = 0;
    private static var _twistedSession:String="";
    private static var _cookie:Array/*of cookieItem*/=[];
    private static var _restUrl:String = "http://"
        + PokerClient.SERVER_HOST 
        + ":" + PokerClient.SERVER_PORT 
        + "/POKER_REST";

    public static var UserSerial:int;

    public static function set cookie(cookie:Array):void
    {
        if (_cookie.length<=0 && cookie.length>0)
        {
            _cookie = cookie;
            /* store(); */
        }
    }

    public static function flush():void
    {
          _localeSession = SharedObject.getLocal("session");
          _localeSession.clear();
          _cookie = [];
    }

    public static function get cookie():Array
    {

        /*if (_cookie.length<=0)
        {
            localeSession = SharedObject.getLocal("session");

            if (localeSession.data.session!=null)
            {
                _cookie = localeSession.data.session;
            }
        }*/

        return _cookie;
    }

    private static function store():void
    {
        _localeSession = SharedObject.getLocal("session");
        _localeSession.data.session = _cookie;
        _localeSession.flush();
    }

    private static function generateAuthHash():String
    {
        return SHA1.hash(_restUrl + Math.random());
    }
    
    private  static function generateUidHash():String
    {
        return SHA1.hash(SHA1.hash(_restUrl) + Math.random());
    }
    
    public static function getUrl():String
    {
        if (_auth_hash == "") {
            _auth_hash = generateAuthHash();
            _session_uid = generateUidHash();
        }   
 
        return _restUrl + "?auth=" + _auth_hash + "&uid=" + _session_uid;
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
}

}
