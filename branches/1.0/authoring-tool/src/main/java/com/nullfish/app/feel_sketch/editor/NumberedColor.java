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

import java.awt.Color;
import java.util.HashMap;
import java.util.Map;

/**
 * An indexed color class.
 * @author shunji
 *
 */
@SuppressWarnings("serial")
public class NumberedColor extends Color {
	private final int id;

	private static Map<Integer, NumberedColor> idColorMap = new HashMap<Integer, NumberedColor>();
	
	public static final NumberedColor BROWN = new NumberedColor(1, 0x7C6035);
	public static final NumberedColor RED = new NumberedColor(2, 0xCC0000);
	public static final NumberedColor ORANGE = new NumberedColor(3, 0xFFB74C);
	public static final NumberedColor YELLOW = new NumberedColor(4, 0xFFFF00);
	public static final NumberedColor GREEN = new NumberedColor(5, 0x00FF41);
	public static final NumberedColor BLUE = new NumberedColor(6, 0x0C00CC);
	public static final NumberedColor PURPLE = new NumberedColor(7, 0xC400CC);
	public static final NumberedColor GRAY = new NumberedColor(8, 0x8C8C8C);
	public static final NumberedColor WHITE = new NumberedColor(9, 0xFAFAFA);
	public static final NumberedColor BLACK = new NumberedColor(10, 0x262626);
	public static final NumberedColor FLESH = new NumberedColor(11, 0xEAD2AD);
	public static final NumberedColor OPTIC_PINK = new NumberedColor(12, 0xefabcd);
	public static final NumberedColor OPTIC_BLUE = new NumberedColor(32, 0xabfedc);
	public static final NumberedColor OPTIC_GREEN = new NumberedColor(33, 0xadefbc);
	public static final NumberedColor OPTIC_YELLOW = new NumberedColor(34, 0xedfabc);
	public static final NumberedColor OPTIC_PURPLE = new NumberedColor(35, 0xcfabde);
	public static final NumberedColor OPTIC_ORANGE = new NumberedColor(36, 0xfcdeab);

	public int getId() {
		return id;
	}

	private NumberedColor(int id, int rgb) {
		super(rgb);
		this.id = id;
		idColorMap.put(id, this);
	}
	
	public static NumberedColor getColor(int id) {
		return idColorMap.get(id);
	}

}
