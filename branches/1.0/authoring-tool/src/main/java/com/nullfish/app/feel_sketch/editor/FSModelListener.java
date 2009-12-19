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
 * A listener class of FSModel
 * @author shunji
 *
 */
public interface FSModelListener {
	/**
	 * It is called when a layer is added.
	 * @param model
	 */
	public void layerAdded(LayerModel model);
	
	/**
	 * It is called when a layer is removed
	 * @param model
	 */
	public void layerRemoved(LayerModel model);

	/**
	 * It is called when the layer order is changed.
	 * @param model
	 */
	public void layerOrderChanged();

	/**
	 * It is called when the background image is changed.
	 */
	public void backgroundImageChanged();
	
	/**
	 * It is called when the size is changed.
	 */
	public void sizeChanged();
}
