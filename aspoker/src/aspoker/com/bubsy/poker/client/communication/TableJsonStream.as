package aspoker.com.bubsy.poker.client.communication
{
import aspoker.com.bubsy.poker.client.event.TableEvent;
import aspoker.com.bubsy.poker.client.event.TableListEvent;

import com.adobe.serialization.json.JSON;
import com.bubzy.utils.Logger;

public class TableJsonStream extends JsonStream
{

    public function TableJsonStream()
    {
        super();
    }

    override protected function _dispatchEvent(pokerPacket:Object):void
    {
           Logger.log(pokerPacket.type);
        trace(JSON.encode(pokerPacket));
        switch(pokerPacket.type)
        {
            case "PacketPokerTable":
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerTable,
                    pokerPacket
                    )
                );
                break;
            }

            case "PacketPokerBuyInLimits":
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerBuyInLimits,
                    pokerPacket
                    )
                );
                break;
            }

            case "PacketPokerBatchMode":
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerBatchMode,
                    pokerPacket
                    )
                );
                break;
            }

            case "PacketPokerSeats":
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerSeats,
                    pokerPacket
                    )
                );
                break;
            }

            case "PacketPokerStreamMode":
            {
                dispatchEvent(
                    new TableEvent(
                    TableEvent.onPacketPokerStreamMode,
                    pokerPacket
                    )
                );
                break;
            }

            default: trace("unknown paket");
         }
    }

    public function tableJoin(gameid:int,userSerial:int):void
    {
        var packetPokerTableJoin:Object = {};
        packetPokerTableJoin.type = "PacketPokerTableJoin";
        packetPokerTableJoin.game_id = gameid;
        packetPokerTableJoin.serial = userSerial;
        sendREST(packetPokerTableJoin);
    }

}
}