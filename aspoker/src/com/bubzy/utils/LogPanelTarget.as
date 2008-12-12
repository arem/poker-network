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
            setPanelTarget(console);
            this.level = LogEventLevel.ALL;
            this.includeDate = true;
            this.includeTime = true;
            this.includeCategory = true;
            this.includeLevel = true;
        }

        override mx_internal function internalLog(message : String) : void
        {
            this.console.text += message + "\n";
        }
    }
}