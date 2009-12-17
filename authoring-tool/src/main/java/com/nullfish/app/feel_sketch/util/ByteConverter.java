/**
***                      "Feel Sketch" authoring tool.
***    Copyright (C) 2009, Shunji Yamaura
***    Copyright (C) 2009, Noritsuna Imamura (noritsuna@siprop.org)
***
***    This program is free software: you can redistribute it and/or modify
***    it under the terms of the GNU General Public License as published by
***    the Free Software Foundation, either version 3 of the License, or
***    (at your option) any later version.
***
***    This program is distributed in the hope that it will be useful,
***    but WITHOUT ANY WARRANTY; without even the implied warranty of
***    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
***    GNU General Public License for more details.
***
***    You should have received a copy of the GNU General Public License
***    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

package com.nullfish.app.feel_sketch.util;

/**
 * An utility class which converts int or long value to byte array.
 * @author shunji
 *
 */
public class ByteConverter {
	public static byte[] convert(int number, int length) {
		byte[] rtn = new byte[length];
		for(int i=0; i<length; i++) {
			rtn[length - 1 - i] = (byte)(number % 256);
			number >>= 8;
		}
		return rtn;
	}

	public static byte[] convert(long number, int length) {
		byte[] rtn = new byte[length];
		for(int i=0; i<length; i++) {
			rtn[i] = (byte)(number % 256);
			number >>= 8;
		}
		return rtn;
	}
}
