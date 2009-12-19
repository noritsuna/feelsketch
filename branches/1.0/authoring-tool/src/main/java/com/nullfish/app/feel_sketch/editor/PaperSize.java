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

package com.nullfish.app.feel_sketch.editor;

/**
 * Enumeration of paper size.
 * @author shunji
 *
 */
public enum PaperSize {
	l(1, 191), card(2, 180), postCard(3, 172), a3(4, 180), a4(5, 180), b4(6, 180), b5(7, 180);
	
	private int value;
	
	private int height;
	
	private PaperSize(int value, int height) {
		this.value = value;
		this.height = height;
	}
	
	public int getValue() {
		return value;
	}
	
	public int getHeight() {
		return height;
	}
	
	public PaperSize getFromValue(int value) {
		switch(value) {
		case 1 : return l;
		case 2 : return card;
		case 3 : return postCard;
		case 4 : return a3;
		case 5 : return a4;
		case 6 : return b4;
		case 7 : return b5;
		default : return null;
		}
	}
}
