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
    import mx.core.mx_internal;
    import mx.logging.ILoggingTarget;
    import mx.logging.Log;
    import mx.logging.LogEventLevel;
    import mx.logging.targets.LineFormattedTarget;
    use namespace mx_internal;

    public class LogPanelTarget extends LineFormattedTarget
    {
        private var console : TextArea;
        private static var _console:TextArea;

        public function setPanelTarget(console:TextArea):void
        {
            if (console)
            {
                this.console = console;
            }
        }

        public function LogPanelTarget(console:TextArea):void
        {
            super();
            if (console)
                setPanelTarget(console);
            this.level = LogEventLevel.ALL;
            this.includeDate = true;
            this.includeTime = true;
            this.includeCategory = true;
            this.includeLevel = true;
        }

        override mx_internal function internalLog(message : String) : void
        {
            if (console)
                this.console.text += message + "\n";
        }
    }
}