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
 * An listener interface of an model of picture element.
 * @author shunji
 *
 */
public interface ElementModelListener {
	/**
	 * It will be called when property is changed.
	 * @param name	property name
	 * @param oldValue	old value
	 * @param newValue	new value
	 */
	public void changed(String name, Object oldValue, Object newValue);

	/**
	 * It will be called when a data is added.
	 * @param name	property name
	 * @param added	the data which is added.
	 */
	public void added(String name, Object added);

	/**
	 * It will be called when a data is added.
	 * @param name	property name
	 * @param removed the data which is removed.
	 */
	public void removed(String name, Object removed);
}
