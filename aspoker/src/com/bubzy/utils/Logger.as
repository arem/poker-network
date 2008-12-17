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

package com.bubzy.utils
{
    import mx.controls.TextArea;
    import mx.core.Application;
    import mx.logging.ILogger;
    import mx.logging.ILoggingTarget;
    import mx.logging.Log;
    import mx.logging.LogEventLevel;

    public class Logger
    {
        private static var _logger:ILogger = Log.getLogger("default");
        private static var _instance:Logger=null;
        private static var target:ILoggingTarget=null;

        public static function log(str:String,console:TextArea=null):void
        {
            if (!_instance)
            {
                _instance = new Logger();

                if (Application.application.hasOwnProperty("log"))
                {
                    addTarget(Application.application.log);
                } else {
                    addTarget(new TextArea());
                }
            }
            _logger.log(LogEventLevel.DEBUG,str);
        }

         public static function addTarget(console:TextArea):void
         {
           target = new LogPanelTarget(console);
           Log.addTarget(target);
        }

        public function Logger()
        {
            if (_instance != null)
            {
                throw new Error(
                    "Only one Singleton instance should be instantiated"
                );
            }
        }
    }
}