/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
 *
 * This file is part of jbot.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

import java.net.*;
import java.io.*; 

class net {
	int port;
	String server;
	Socket socket;
	DataInputStream in;
	DataOutputStream out;

	public net() {
		this.port   = 9999       ;
		this.server = "localhost";
		this.socket = null       ;
		this.in     = null       ;
		this.out    = null       ;
	}

	public net(String server, int port) {
		this.server = server;
		this.port   = port  ;
		this.socket = null  ;
		this.in     = null  ;
		this.out    = null  ;
	}

	public void connect() throws IOException {
		this.socket = new Socket(this.server,this.port);

		OutputStream rawOut = socket.getOutputStream();
		InputStream rawIn   = socket.getInputStream();

		BufferedOutputStream buffOut = new BufferedOutputStream(rawOut);

		this.out = new DataOutputStream(buffOut);
		this.in  = new DataInputStream(rawIn);
	}


	public void sendMessage(int type, byteArray b) throws IOException {
		byte [] myBytes = b.getBytes();
		byte r[]        = new byte[4];

		r[3] = (byte) ((type      ) & 0x000000FF);
		r[2] = (byte) ((type >>  8) & 0x000000FF);
		r[1] = (byte) ((type >> 16) & 0x000000FF);
		r[0] = (byte) ((type >> 24) & 0x000000FF);

		out.write(r,0,r.length);
		out.flush();

		r[3] = (byte) ((myBytes.length      ) & 0x000000FF);
		r[2] = (byte) ((myBytes.length >>  8) & 0x000000FF);
		r[1] = (byte) ((myBytes.length >> 16) & 0x000000FF);
		r[0] = (byte) ((myBytes.length >> 24) & 0x000000FF);

		out.write(r,0,r.length);
		out.flush();

		out.write(myBytes,0,myBytes.length);
		out.flush();
	}

	public int readMessage(byteArray b) throws IOException {
		int size, type;
		byte[] temp = new byte[4];
		byte[] buf;

		in.read(temp, 0, 4);
		type =  (int)((temp[3] < 0 ? 256+temp[3] : temp[3])      );
		type += (int)((temp[2] < 0 ? 256+temp[2] : temp[2]) <<  8);
		type += (int)((temp[1] < 0 ? 256+temp[1] : temp[1]) << 16);
		type += (int)((temp[0] < 0 ? 256+temp[0] : temp[0]) << 24);

		in.read(temp, 0, 4);
		size =  (int)((temp[3] < 0 ? 256+temp[3] : temp[3])      );
		size += (int)((temp[2] < 0 ? 256+temp[2] : temp[2]) <<  8);
		size += (int)((temp[1] < 0 ? 256+temp[1] : temp[1]) << 16);
		size += (int)((temp[0] < 0 ? 256+temp[0] : temp[0]) << 24);

		buf = new byte[size];
		in.read(buf,0,size);

		b.appendBytes(size,buf);
		return type;
	}

	public void disconnect() throws IOException {
		if (this.socket != null) {
			this.socket.close();
		}
	}

}
