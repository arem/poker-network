package aspoker.com.bubsy.poker.client.model
{

import aspoker.com.bubsy.poker.client.communication.PokerConnection;
import aspoker.com.bubsy.poker.client.event.TableListEvent;

import flash.events.TimerEvent;
import flash.utils.Timer;

import mx.controls.DataGrid;


public class PokerTableList
{
    private var _pokerConnection:PokerConnection = new PokerConnection();
    public var data:Array = [];
    private var ticker:Timer;
    private var tableGrid:DataGrid;

    public function PokerTableList(tableGrid:DataGrid)
    {
        this.tableGrid = tableGrid ;
        ticker = new Timer(6000);
        ticker.addEventListener(TimerEvent.TIMER, doStep);
        ticker.start();
         _pokerConnection.addEventListener(
            TableListEvent.onPacketPokerTableList,
            _onPacketPokerTableList);
    }

    private function doStep(evt:TimerEvent):void
    {
        _pokerConnection.getTables();
    }

    private function _onPacketPokerTableList(evt:TableListEvent):void
    {
        this.data = evt.packet.packets;
        tableGrid.dataProvider=data;
    }
}

}