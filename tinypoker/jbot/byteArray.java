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

public class byteArray {
	byte[] data;
	int size;
	int capacity;
	int position;

	public byteArray() {
		data     = new byte[10];
		capacity = 10;
		size     = 0;
		position = 0;
	}

	public byteArray(int cap) {
		data     = new byte[cap];
		capacity = cap;
		size     = 0;
		position = 0;
	}

	public void appendBytes(int size, byte [] data) {
		if (this.data == null || size == 0)
			return;

		/* check if we have enough space */
		if ((size + this.size) > this.capacity) {
			/* make more space */
			byte [] newData = new byte[this.capacity+size];

			for (int i = 0; i < this.data.length; i++)
				newData[i] = this.data[i];

			this.data      = newData;
			this.capacity += size;
		}

		for (int i = 0; i < size; i++)
			this.data[i+this.size] = data[i];
		this.size += size;
	}

	public void appendString(String str) {
		if (str == null)
			return;

		byte [] term = {'\0'};

		appendBytes(str.length(),str.getBytes());
		appendBytes(1,term);
	}

	public void appendInt(int i) {
		byte r[] = new byte[4];

		r[3] = (byte) ((i      ) & 0x000000FF);
		r[2] = (byte) ((i >>  8) & 0x000000FF);
		r[1] = (byte) ((i >> 16) & 0x000000FF);
		r[0] = (byte) ((i >> 24) & 0x000000FF);

		appendBytes(4,r);
	}

	public byte[] getBytes() {
		return this.data;
	}

	public String readString() {
		StringBuffer buf = new StringBuffer();

		while (this.data[position] != (byte)0) {
			buf.append((char)this.data[position]);
			position++;
		}

		position++; /* '\0' */

		String str = new String(buf);
		return str;
        }

	public int readInt() {
		int i;

		i =  (int)((this.data[position+3] < 0 ? 256+this.data[position+3] : this.data[position+3])      );
		i += (int)((this.data[position+2] < 0 ? 256+this.data[position+2] : this.data[position+2]) <<  8);
		i += (int)((this.data[position+1] < 0 ? 256+this.data[position+1] : this.data[position+1]) << 16);
		i += (int)((this.data[position+0] < 0 ? 256+this.data[position+0] : this.data[position+0]) << 24);

		this.position += 4;

		return i;
	}

}
