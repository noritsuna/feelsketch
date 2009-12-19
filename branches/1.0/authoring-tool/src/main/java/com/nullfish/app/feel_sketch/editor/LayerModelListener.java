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

public interface LayerModelListener {
	/**
	 * It is called when a model is added in a layer.
	 * @param model
	 */
	public void modelAdded(ElementModel model);
	
	/**
	 * It is called when a model is removed from a layer.
	 * @param model
	 */
	public void modelRemoved(ElementModel model);
 
	/**
	 * It is called when the flag of visibility of layer in the PM code.
	 */
	public void toBePMCodedChanged();
	
	/**
	 * It is called when the effect is changed.
	 */
	public void effectChanged();
	
	/**
	 * It is called when the name is changed.
	 */
	public void nameChanged();
}
