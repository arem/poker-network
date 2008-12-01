package com.bubzy.utils
{

	import mx.controls.TextArea;
	import mx.core.mx_internal;
	import mx.logging.LogEventLevel;
	import mx.logging.targets.LineFormattedTarget;   
	use namespace mx_internal;

	public class LogPanelTarget extends LineFormattedTarget
	{
        private var console : TextArea;

        public function LogPanelTarget(console : TextArea)
        {
			super();
			this.console = console;
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