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

import aspoker.com.bubsy.poker.client.communication.TableJsonStream;
import aspoker.com.bubsy.poker.client.event.TableListEvent;
import aspoker.com.bubsy.poker.client.util.PollTimer;
import aspoker.com.bubsy.poker.client.PokerClient;

import com.bubzy.utils.Logger;
import flash.events.TimerEvent;
import mx.controls.DataGrid;

public class TableList extends PollTimer
{
    private var _pokerConnection:TableJsonStream;
    private var _data:Array = [];
    private var _tableGrid:DataGrid;
    private var _playersCount:int=0;
    private var _tablesCount:int=0;

    public function TableList(tableGrid:DataGrid)
    {
        _pokerConnection = PokerClient.stream;
        TableJsonStream.setLobby(this);
        _tableGrid = tableGrid ;
        
         refreshTablelist();
    }

    public function get playersCount():int
    {
        return _playersCount ;
    }

    public function get tablesCount():int
    {
        return _tablesCount;
    }

    private function refreshTablelist():void
    {
        _pokerConnection.getTables();
    }

    protected override function doStep(evt:TimerEvent):void
    {
        refreshTablelist();
    }

    public function onPacketPokerTableList(packet:Object):void
    {
        _data = packet.packets;
        _playersCount = packet.players;
        _tablesCount = packet.tables;
        Logger.log("playersCount:" + _playersCount
                     + " tablesCount:" + _tablesCount);
        _tableGrid.dataProvider = _data;
    }
}
}
