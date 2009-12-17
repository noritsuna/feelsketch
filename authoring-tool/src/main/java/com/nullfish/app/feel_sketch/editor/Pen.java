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

import java.awt.AlphaComposite;
import java.awt.Composite;

/**
 * Enumeration of pen types.
 * 
 * @author shunji
 *
 */
public enum Pen {
	narrow(1, 1, 1.0f),
	midium(2, 3, 1.0f), 
	wide(3, 6, 1.0f),
	veryWide(4, 10, 1.0f), 
	highliterNarrow(5, 3, 0.5f), 
	highliterMidium(6, 6, 0.5f), 
	highliterWide(7, 10, 0.5f); 

	/**
	 * Pen id
	 */
	private int id;
	
	/**
	 * Pen width(not pixel)
	 */
	private int width;
	
	/**
	 * Pen alpha value.
	 */
	private float alpha;
	
	/**
	 * Alpha composite of this pen.
	 */
	private Composite composite;
	
	private Pen(int id, int width, float alpha) {
		this.id = id;
		this.width = width;
		this.alpha = alpha;
		composite = AlphaComposite.getInstance(AlphaComposite.SRC_OVER, alpha);
	}
	
	public int getId() {
		return id;
	}
	
	public int getWidth() {
		return width;
	}
	
	public float getAlpha() {
		return alpha;
	}
	
	public int getRealWidth() {
		return width * 640 / 254;
	}
	
	public Composite getComposite() {
		return composite;
	}
}
