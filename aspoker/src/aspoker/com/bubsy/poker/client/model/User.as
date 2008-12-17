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

import aspoker.com.bubsy.poker.client.communication.UserJsonStream;
import aspoker.com.bubsy.poker.client.event.LoginEvent;

import flash.events.EventDispatcher;

public class User extends EventDispatcher
{
    private var _userPassword:String= "";
    private var _userName:String = "";
    private var _userSerial:int= -1;
    private var _stream:UserJsonStream = new UserJsonStream();
    private var _tablesList:Array/* of poker tables */=[];

    public function User()
    {
        _stream.addEventListener(
            LoginEvent.onPacketAuthOk,
            _loginSuccessfull
        );

        _stream.addEventListener(
            LoginEvent.onPacketAuthRefused,
            _loginRefused
        );

        _stream.addEventListener(
            LoginEvent.onPacketSerial,
            onPacketSerial
        );
    }

    private function onPacketSerial(evt:LoginEvent):void
    {
        _userSerial = evt.userSerial;
        trace("userserial"+_userSerial);
    }

    private function _loginSuccessfull(evt:LoginEvent):void
    {
        dispatchEvent(
            new LoginEvent(
            LoginEvent.onPacketAuthOk,
            _userSerial
            )
        );
    }

    private function _loginRefused(evt:LoginEvent):void
    {
        dispatchEvent(
            new LoginEvent(
            LoginEvent.onPacketAuthRefused
            )
        );
    }

    public function tableJoin(gameId:int):void
    {
        //Logger.log(UserSerial +  " join table" + gameid);
        _tablesList[gameId] = new Table(gameId);
    }

    public function loggin(userName:String,userPassword:String):void
    {
        _userName = userName;
        _userPassword = userPassword;
        _stream.loggin(_userName,_userPassword);
    }

    public function logout():void
    {
        _userPassword= "";
        _userName = "";
        _userSerial= -1;
        _stream.logout();
    }

    public function get userName():String
    {
        return _userName;
    }

    public function get userSerial():int
    {
        return _userSerial;
    }
}
}