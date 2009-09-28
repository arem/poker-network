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

package aspoker.com.bubsy.poker.client.util
{
import flash.events.EventDispatcher;
import flash.events.TimerEvent;
import flash.utils.Timer;

    public class PollTimer extends EventDispatcher
    {
        private var _poll:Timer;
        protected static var pollFrequency:int = 5000;


        public function PollTimer()
        {
            _poll = new Timer(pollFrequency);
            _poll.addEventListener(TimerEvent.TIMER, doStep);
            _poll.start();
        }

        public function startPoll():void
        {
            _poll.start();
        }

        protected function doStep(evt:TimerEvent):void
        {
            return;
        }

        public function stopPoll():void
        {
            _poll.stop();
        }
    }
}
