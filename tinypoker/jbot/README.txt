This code is provided by the Bishop's University Computer Science Club
to participants in its programming competition. The reason for providing 
the code is to make it easier for students and alumni with limited 
network application programming experience to participate. It also 
allows contestants to get working on the AI part faster because the
network code is done already.

This package contains 4 parts: this readme, protocol documentation, 
network code, and byte array code. This readme will give information 
about the code contained in this package as well as some advice on its 
usage. Below are a few paragraphs that describe the general ideas needed 
to understand how to use the code. Following the explanation paragraphs 
is a code example. The example code shows you how to authenticate your 
client.

Because of the design of the network protocol and the lack of 
appropriate library function, a special set of byte array functions have 
been included. The byte arrays are variable sized arrays that 
automatically expand if the capacity is exceeded. They are used for 
storing Strings and integers. The format is dependent upon the protocol.
See the protocol documentation for the different message formats.

Strings are stored in the byte array's as they are in C (an array of 
bytes followed by the null byte '\0'). Integers are stored in binary in 
network byte order and are always 32 bits (4 bytes) and signed. Accessing
Strings and integers from a byte array is as simple as calling readString()
or readInt(). Storing strings and integers is done with appendString() 
and appendInt().

This package also includes code to take care of network communication. 
The code has four basic functions. It allows you to connect to a server, 
send messages, read messages, and disconnect from the server. When your 
program starts and before it attempts to read or write any messages it 
must be connected to a server. After your program is done communicating 
and before your program terminates it must disconnect from the server.

public class example {
	static net n;
	public static final int JOIN_GAME = 20;
	public static final int GOODPASS  = 21;

	public static int authenticate() throws Exception {
		int type;

		/* create a new byteArray */
		byteArray b = new byteArray(32);

		/* pack the login info */
		b.appendString("username");	/* username */
		b.appendString("password");	/* password */
		b.appendInt(1);			/* protocol version */
		b.appendString("client_name");	/* client name */

		/* send a JOIN_GAME request to the server */
		n.sendMessage(JOIN_GAME,b);

		/* get a response */
		b    = new byteArray(16);
		type = n.readMessage(b);

		/* Check if we are authenticated */
		if (type == GOODPASS) {
			return 1;
		} else {
			return 0;
		}
	}

	public static void main(String [] args) throws Exception {
		/* server info here */
		n = new net("localhost",9999);

		n.connect(); /* connect to the server */

		/* Authenticate! */
		if (authenticate() == 1) {
			System.out.println("Authenticated!");
		} else {
			System.out.println("Not Authenticated!");
		}

		n.disconnect(); /* disconnect from the server */
	}
}
