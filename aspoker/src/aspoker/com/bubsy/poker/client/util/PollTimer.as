package aspoker.com.bubsy.poker.client.util
{
import flash.events.EventDispatcher;
import flash.events.TimerEvent;
import flash.utils.Timer;

    public class PollTimer extends EventDispatcher
    {
        private var _poll:Timer;
        public static var pollFrequency:int = 10000;


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
            _poll.start();
        }
    }
}