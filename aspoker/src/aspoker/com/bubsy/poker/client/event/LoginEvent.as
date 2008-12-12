package aspoker.com.bubsy.poker.event
{

import aspoker.com.bubsy.poker.model.PokerSession;
import aspoker.com.bubsy.poker.model.PokerUser;

import flash.events.Event;

public class LoginEvent extends Event
{
    public static const onPacketAuthOk:String = "PacketAuthOk";
    public static const onPacketSerial:String = "PacketSerial";
    public static const onPacketAuthRefused:String = "PacketAuthRefused";
    public var packet:Object;

    public function LoginEvent(type:String,data:Object=null,
        header:Array=null
        )
    {
        this.packet = data ;

        switch(type)
        {
            case "PacketAuthOk": break;
            case "PacketSerial":
            {
                PokerUser.UserSerial = data.serial;
                break;
            }
            default: trace(type);
        }
        super(type);
    }
}

}