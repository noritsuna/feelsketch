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

import java.io.IOException;

/**
 * An interface of a model of picture elements.
 * @author shunji
 *
 */
public interface ElementModel {
	/**
	 * Adds an listener object to this object.
	 * @param listener
	 */
	public void addListener(ElementModelListener listener);

	/**
	 * Removes an listener object from this object.
	 * @param listener
	 */
	public void removeListener(ElementModelListener listener);

	/**
	 * Removes all listener objects from this object.
	 */
	public void clearListener();
	
	/**
	 * Returns byte size of this object as PM data.
	 * @return
	 */
	public int getMemorySize();
	
	/**
	 * Returns true if it is to be exported as PM data.
	 * @return
	 */
	public boolean isToBePMCode();
	
	/**
	 * Returns byte array of exporting data.
	 * @param model
	 * @return
	 */
	public byte[] toBytes(FSModel model) throws IOException;
}
