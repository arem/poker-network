package aspoker.com.bubsy.poker.client.model
{

import mx.messaging.channels.StreamingAMFChannel;

public class PokerSession
{
    private static var _sessionCount:int = 0;
    private static var _twistedSession:String="";
    private static var restUrl:String = "http://192.168.0.7:19382/REST";
    public static var UserSerial:int;

    public static function setCookie(twistedSession:String):void
    {
        _twistedSession = twistedSession ;
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