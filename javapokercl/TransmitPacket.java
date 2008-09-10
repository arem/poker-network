import org.apache.xmlrpc.client.*;
import java.util.*;
import java.net.*;

public class TransmitPacket {
	
    public static void main( String args[] ) throws Exception {
        XmlRpcClientConfigImpl config = new XmlRpcClientConfigImpl();
        config.setServerURL(new URL("http://127.0.0.1:19382/RPC2"));
        XmlRpcClient client = new XmlRpcClient();
        client.setConfig(config);

        Hashtable<String,String> packet = new Hashtable<String,String>(); 
        packet.put("type", "PacketPokerTableSelect");         
        packet.put("string", "");
        
        String mode = new String("use sessions");
        String extMethod = new String("handlePacket");
        
        Object[] params = new Object[]{mode,packet};
        Object[] result = (Object[]) client.execute(extMethod, params);
        
        Map<String,Object> map = (Map<String,Object>)result[0]; 
        Object[] q = (Object[])map.get("packets");

        System.out.println("Working tables:");
        for (int i = 0; i < q.length; i++) {
        	Map table = (Map<String,Object>)q[i];
            System.out.println(table.get("name"));
		}
    }
}
