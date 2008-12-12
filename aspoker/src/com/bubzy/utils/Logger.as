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
                addTarget(Application.application.log);
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