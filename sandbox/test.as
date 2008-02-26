package {
 import flash.net.*;
 import flash.util.*;
 import flash.events.*;
 import flash.display.*;
 import flash.utils.*;
 import com.adobe.serialization.json.JSON;

 public class table extends MovieClip {

  private var serial:int=0;

  public function registerSerial(serial:String) {
   var sharedReg:SharedObject=SharedObject.getLocal("register","/");
   sharedReg.data.serial=serial;
  }
  
  public function Loggin() {
   var packetLogin:Object=new Object  ;
   packetLogin.password="bidon";
   packetLogin.name="xavier";
   packetLogin.type="PacketLogin";

   sendREST(packetLogin);
   
/*
   var packetPokerGetPersonalInfo:Object = new Object();
   packetPokerGetPersonalInfo.type="PacketPokerGetPersonalInfo";
   packetPokerGetPersonalInfo.serial = 544;
   
   sendREST(packetPokerGetPersonalInfo);
*/   

   // join table
   var packetPokerTableJoin = {
    type:"PacketPokerTableJoin",
    game_id:100,
    serial:544
   };
   sendREST(packetPokerTableJoin);
   
   // seat
   var packetPokerSeat = {
    type:"PacketPokerSeat",
    seat:-1,
    game_id:100,
    serial:544
   }
   sendREST(packetPokerSeat);
   
   // buy in
   var packetPokerBuyIn = {
    type:"PacketPokerBuyIn",
    amount:200000,
    game_id:100,
    serial:544  
   }
   sendREST(packetPokerBuyIn);
   
   // ready
   var packetPokerSit = {
    type:"PacketPokerSit",
    game_id:100,
    serial:544 
   }
   sendREST(packetPokerSit);

   
   /*
   var packetPokerTableSelect:Object=new Object;
   packetPokerTableSelect.type = "PacketPokerTableSelect";
   packetPokerTableSelect.string = "";
   packetPokerTableSelect.serial = 544;
   
   sendREST(packetPokerTableSelect);
   
      
   var packetLogout:Object = new Object();
   packetLogout.type="PacketLogout";
   sendREST(packetLogout);
   */

  }
  
  public function test(event:Event)
  {
    trace(event.target.data);
  }
  
  public function sendREST(packet:Object) {
   
   var header:URLRequestHeader=null;
    var loader:URLLoader=new URLLoader  ; 
   
   header=new URLRequestHeader("Content-Type","text/xml; charset=latin-1");
   loader.dataFormat=URLLoaderDataFormat.TEXT;
   //loader.addEventListener(ProgressEvent.PROGRESS,test);
   loader.addEventListener(Event.COMPLETE,dispatch);
   loader.addEventListener(Event.CANCEL,handleError);
   
   var request:URLRequest=new URLRequest  ;   
   request.method=URLRequestMethod.POST;
   request.url="http://192.168.0.100:19382/REST?session=yes";
   request.requestHeaders.push(header);
   request.data=JSON.encode(packet);
   
   try {
    loader.load(request);
   } catch (error:Error) {
    trace("Unable to load request");
   }
  }

  public function handleError():void {
   trace("IT FAILEDED");
  }
  public function dispatch(event:Event) {
   trace(event.target.data);
   var results:Array=JSON.decode(event.target.data)  as  Array;

   for (var i=0; i < results.length; i++) {
    if (results[i] != null) {
     trace(results[i].type);
     // TODO load object f(type);
    }
   }
  }
 }
}

