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

package aspoker.com.bubsy.poker.client
{
import aspoker.com.bubsy.poker.client.communication.JsonStreamProxy;
import aspoker.com.bubsy.poker.client.model.User;
import flash.events.*;
import flash.net.*;

public class PokerClient
{
    public static const VIEW_IS_BOARD:int=1;
    public static const VIEW_IS_TABLE:int=1;
    public static const VIEW_IS_CASHIER:int=1;

    private static var _user:User = null ;;
    private static var _currentState:int = VIEW_IS_BOARD;

    public static var SERVER_HOST:String = "";
    public static var SERVER_PORT:int;
    public static var CARDS_PATH:String = "";
    public static var CARDS_PREFIX:String = "";
    public static var IMAGE_PATH:String = CARDS_PATH + CARDS_PREFIX;
    
    private static var _actionJsonStream:JsonStreamProxy = new JsonStreamProxy();
    private static var _loader:URLLoader = new URLLoader();
    private static var _request:URLRequest = new URLRequest("config.xml");
    private static var _configIsLoaded:Boolean = false;    
 
    public function PokerClient()
    {
    }
    
    public static function get confiIsLoaded():Boolean
    {
        return _configIsLoaded;
    }


     public static function loadConfig():void
    {
         _loader.addEventListener(Event.COMPLETE,_onComplete);
        
         try {
            _loader.load(_request);
         } catch (error:Error) {
            trace("Unable to load requested document.");
         }
    }

    private static function _onComplete(event:Event):void
    {
        trace("configuration loaded");
        var loader:URLLoader = event.target as URLLoader;
        if (loader != null)
        {
            var externalXML:XML = new XML(loader.data);
            SERVER_PORT = externalXML.restport;        
            SERVER_HOST = externalXML.resthost;
            CARDS_PATH = externalXML.imagecardspath;
            CARDS_PREFIX = externalXML.imagecadsprefix;
            IMAGE_PATH = CARDS_PATH + CARDS_PREFIX;
            _configIsLoaded = true;
        }
        else
        {
            trace("loader is not a URLLoader!");
            _configIsLoaded = false;
        }
    }

    static public function get stream():JsonStreamProxy
    {
        return _actionJsonStream;
    }

    public static function get user():User
    {
       if (_user == null) {
            _user = new User(); 
        }
         
        return _user;
    }
}
}
