package aspoker.com.bubsy.poker.client.event
{

import flash.events.Event;

public class TableListEvent extends Event
{
    public static const onPacketPokerTableList:String = "PacketPokerTableList";
    public var packet:Object;

    public function TableListEvent(type:String,data:Object=null)
    {
        this.packet = data;
        super(type);
    }

}

}