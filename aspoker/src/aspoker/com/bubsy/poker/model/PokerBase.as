package aspoker.com.bubsy.poker.model
{
	import aspoker.com.bubsy.poker.event.LoginEvent;
	
	import com.adobe.serialization.json.JSON;
	import com.bubzy.net.http.HTTPClient;
	import com.bubzy.utils.LogPanelTarget;
	
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.HTTPStatusEvent;
	import flash.net.URLRequest;
	import flash.net.URLRequestHeader;
	
	import mx.core.Application;
	import mx.logging.ILogger;
	import mx.logging.ILoggingTarget;
	import mx.logging.Log;
	import mx.logging.LogEventLevel;

	public class PokerBase extends EventDispatcher
	{
		private static var _restURL:String = "http://192.168.0.7:19382/REST?session=yes";
		private static var _instance:PokerBase;
		private static var _httpClient:HTTPClient;
		private static var _logger:ILogger = Log.getLogger("default"); 
		private static var _cookie:String;
		private static var _userSerial:int;
		
		[Event(name="login", type="LoginEvent.LOGIN")]
		
		public static function getInstance():PokerBase
		{
			if ( _instance == null) {
				_instance = new PokerBase();
				_instance._init();
			}
			return _instance;
		}
		
		public function PokerBase():void
		{
			if ( _instance != null )
				throw new Error( "Only one Singleton instance should be instantiated" );
		}
		
		private function _init():void
		{
			_httpClient = new HTTPClient();
			_httpClient.addEventListener("complete", onComplete);
			_httpClient.addEventListener("httpStatus", onHTTPStatus);
			_httpClient.addEventListener("close", handleError);
	    	
	     	var target:ILoggingTarget = new LogPanelTarget(Application.application.log);
			Log.addTarget(target); 
		}
		
		private function sendREST(packet:Object):void 
		{
			var request:URLRequest = new URLRequest();
			var authHeader:URLRequestHeader = new URLRequestHeader("Content-Type","text/xml; charset=latin-1");
			
			request.url = _restURL;
			request.requestHeaders.push(authHeader);
			request.data=JSON.encode(packet);
			_httpClient.httpPOST(request);
		}
		
		private function onComplete(event:Event):void
		{
		   var responseContent:String = HTTPClient(event.target).responseContent ;
		   var results:Array = JSON.decode(responseContent);
		    _logger.log(LogEventLevel.DEBUG,responseContent)
		   
		   for (var i:int=0; i < results.length; i++) {
		   		if (results[i] != null) {
		        	switch(results[i].type) {
			   			case "PacketAuthOk" : dispatchEvent(new LoginEvent(LoginEvent.LOGIN)); break;
		   				case "PacketSerial" :  _cookie = results[i].cookie ; _userSerial = results[i].serial; break;
		   				case "PacketPokerTableList" : Application.application.tableinfo.dataProvider = results[i].packets;break;
			   			case "PacketAuthRequest" : break;
		  				case "PacketPokerTable": break;
						case "PacketPokerBuyInLimits":break;
						case "PacketPokerBatchMode":break;
						case "PacketPokerSeats": break;
						case "PacketPokerStreamMode":break;
						case "PacketPokerError":break;
		  				case "PacketPokerPlayerInfo":break;
		  				case "PacketAuthRefused": break;
		  				default : _logger.log(LogEventLevel.DEBUG,results[i].type);
		   			}
		   		}
		   	}
		}
		
		private function handleError():void 
		{
			_logger.log(LogEventLevel.DEBUG, "soket closed");
		}
	  
		private function httpStatusHandler(event:Event):void
		{
			_logger.log(LogEventLevel.DEBUG, event.target.data); 
		}
		
		private function onHTTPStatus(event:HTTPStatusEvent):void 
		{
    		_logger.log(LogEventLevel.DEBUG,"httpStatus " + event.status);
    	}
    	
    	public function seat():void 
    	{
	    	var packetPokerSeat:Object = new Object;
		    packetPokerSeat.type="PacketPokerSeat",
		    packetPokerSeat.seat=1,
	    	packetPokerSeat.game_id=1,
		    packetPokerSeat.serial=4
		    sendREST(packetPokerSeat);
	  	}
	  	
	  	public function setRole():void
	  	{
	    	var packetPokerSeat:Object = new Object;
		    packetPokerSeat.type="PacketPokerSetRole",
		    packetPokerSeat.roles="PLAY";
	    	sendREST(packetPokerSeat);
	    }
	
		public function getTables():void 
		{
			var packetPokerTableSelect:Object=new Object;
			packetPokerTableSelect.type = "PacketPokerTableSelect";
			packetPokerTableSelect.string = "";
			sendREST(packetPokerTableSelect)
		}
  
		public function loggin(username:String,password:String):void 
		{
			var packetLogin:Object=new Object  ;
			packetLogin.password= password;
			packetLogin.name= username;
			packetLogin.type="PacketLogin";
			sendREST(packetLogin);
		}
		
		public function plocale():void 
		{
			var plocale:Object=new Object  ;
			plocale.serial=4;
			plocale.locale="en_US.UTF-8";
			sendREST(plocale);
		}
	  
		public function getPersonalInfo():void 
		{
			var packetPokerGetPersonalInfo:Object = new Object();
			packetPokerGetPersonalInfo.type="PacketPokerGetPlayerInfo";
			//packetPokerGetPersonalInfo.serial = 4;
			sendREST(packetPokerGetPersonalInfo);
		 }
	  
		 public function tableJoin():void 
		 {
			var packetPokerTableJoin:Object = new Object();
			packetPokerTableJoin.type="PacketPokerTableJoin";
			packetPokerTableJoin.game_id=1;
			packetPokerTableJoin.serial=4;
			sendREST(packetPokerTableJoin);
		}
	  
		public function sit():void 
		{
			var packetPokerSit:Object = new Object();
			packetPokerSit.type="PacketPokerSit",
			packetPokerSit.game_id=1,
			packetPokerSit.serial=544 
			sendREST(packetPokerSit);
		} 
	  
		public function buyIn():void 
		{
			var packetPokerBuyIn:Object = new Object();
			packetPokerBuyIn.type = "PacketPokerBuyIn";
			packetPokerBuyIn.amount = 200000;
			packetPokerBuyIn.game_id = 1;
			packetPokerBuyIn.serial= 4;
			sendREST(packetPokerBuyIn);
		}
	}
}