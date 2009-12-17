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

import java.awt.Graphics2D;
import java.awt.Shape;
import java.awt.geom.Point2D;

/**
 * An iinterface of pictre elements.
 * @author shunji
 *
 */
public interface Element {
	/**
	 * Returns true if its shape intersects the shape. 
	 * @param shape
	 * @return
	 */
	public boolean intersects(Shape shape);

	/**
	 * Returns true if its shape contains the point. 
	 * @param p
	 * @return
	 */
	public boolean contains(Point2D p);
	
	/**
	 * Draws this element.
	 * @param g
	 */
	public void paint(Graphics2D g);
	
	/**
	 * Returns the model.
	 * @return
	 */
	public ElementModel getModel();
	
	/**
	 * Returns its shape.
	 * @return
	 */
	public Shape getShape();
	
	/**
	 * Repaints it.
	 */
	public void update();
	
	public int getOrder();
	
	public void setUpdating(boolean updating);
	
	public boolean isUpdating();
}
