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

import com.bubzy.utils.Logger;

import flash.events.TimerEvent;
import flash.utils.Timer;

import mx.controls.DataGrid;

public class TableList
{
    private var _pokerConnection:TableListJsonStream
                = new TableListJsonStream();
    private var _data:Array = [];
    private var _ticker:Timer;
    private var _tableGrid:DataGrid;
    private var _playersCount:int=0;
    private var _tablesCount:int=0;

    public function TableList(tableGrid:DataGrid)
    {
        _tableGrid = tableGrid ;
        _ticker = new Timer(6000);
        _ticker.addEventListener(TimerEvent.TIMER, doStep);
        _ticker.start();
         _pokerConnection.addEventListener(
            TableListEvent.onPacketPokerTableList,
            _onPacketTableList);
    }

    public function get playersCount():int
    {
        return _playersCount ;
    }

    public function get tablesCount():int
    {
        return _tablesCount;
    }

    private function doStep(evt:TimerEvent):void
    {
        _pokerConnection.getTables();
    }

    private function _onPacketTableList(evt:TableListEvent):void
    {
        _data = evt.packet.packets;
           _playersCount = evt.packet.players;
         _tablesCount = evt.packet.tables;
         Logger.log("playersCount:" + _playersCount
                     + " tablesCount:" + _tablesCount);
        _tableGrid.dataProvider = _data;
    }
}

}