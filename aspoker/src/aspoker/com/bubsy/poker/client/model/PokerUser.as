package aspoker.com.bubsy.poker.client.model
{

import aspoker.com.bubsy.poker.client.communication.PokerConnection;
import com.bubzy.utils.Logger;

public class PokerUser
{
    public static var UserPassword:String;
    public static var userName:String;
    public static var UserSerial:int;
    public static var pokerConnection:PokerConnection = new PokerConnection();

	public function PokerUser()
	{

	}

    public static function tableJoin(gameid:int):void
    {
        Logger.log(UserSerial +  " join table" + gameid);
        pokerConnection.tableJoin(gameid,UserSerial);
    }

    public static function loggin():void
    {
        var packetLogin:Object = {};
        packetLogin.password = UserPassword;
        packetLogin.name = userName;
        pokerConnection.loggin(userName,UserPassword);
    }

    public static function logout():void
    {
        pokerConnection.logout();
    }
}

}