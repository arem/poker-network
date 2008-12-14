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

public class User
{
    public static var UserPassword:String;
    public static var userName:String;
    public static var UserSerial:int;
    public static var pokerConnection:UserJsonStream = new UserJsonStream();
	public static var tablesList:Array/* of poker tables */=[];

	public function User()
	{

	}

    public static function tableJoin(gameId:int):void
    {
        //Logger.log(UserSerial +  " join table" + gameid);
      	tablesList[gameId] = new Table(gameId);
    }

    public static function loggin():void
    {
        pokerConnection.loggin(userName,UserPassword);
    }

    public static function logout():void
    {
        pokerConnection.logout();
    }
}

}