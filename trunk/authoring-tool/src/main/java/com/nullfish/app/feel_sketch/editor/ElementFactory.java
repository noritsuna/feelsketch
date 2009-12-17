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

import com.nullfish.app.feel_sketch.editor.elements.bezier_line.BezierLineElement;
import com.nullfish.app.feel_sketch.editor.elements.bezier_line.BezierLineElementModel;
import com.nullfish.app.feel_sketch.editor.elements.image.ImageElement;
import com.nullfish.app.feel_sketch.editor.elements.image.ImageElementModel;

/**
 * The factory class of pictures elements.
 * It creates an element form a model.
 * @author shunji
 *
 */
public class ElementFactory {
	public static Element createElement(ElementModel model, Layer layer) {
		if(model instanceof BezierLineElementModel) {
			return new BezierLineElement((BezierLineElementModel)model, layer);
		} else if(model instanceof ImageElementModel) {
			return new ImageElement((ImageElementModel)model, layer);
		}
		
		throw new IllegalStateException();
	}
}
