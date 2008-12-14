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

import aspoker.com.bubsy.poker.client.communication.TableListJsonStream;
import aspoker.com.bubsy.poker.client.event.TableListEvent;

import flash.events.TimerEvent;
import flash.utils.Timer;

import mx.controls.DataGrid;

public class PokerTableList
{
    private var _pokerConnection:TableListJsonStream
    			= new TableListJsonStream();
    public var data:Array = [];
    private var ticker:Timer;
    private var tableGrid:DataGrid;
    private var players:int=0;
    private var tables:int=0;

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
        data = evt.packet.packets;
        tableGrid.dataProvider = data;
    }
}

}