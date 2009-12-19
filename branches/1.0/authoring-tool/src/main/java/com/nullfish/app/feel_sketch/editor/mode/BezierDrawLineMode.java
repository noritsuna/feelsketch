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

package com.nullfish.app.feel_sketch.editor.mode;

import java.awt.Cursor;
import java.awt.event.MouseEvent;

import com.nullfish.app.feel_sketch.editor.Element;
import com.nullfish.app.feel_sketch.editor.FSPanel;
import com.nullfish.app.feel_sketch.editor.LayerModel;
import com.nullfish.app.feel_sketch.editor.Mode;
import com.nullfish.app.feel_sketch.editor.NumberedColor;
import com.nullfish.app.feel_sketch.editor.Pen;
import com.nullfish.app.feel_sketch.editor.elements.bezier_line.BezierLineElementModel;

/**
 * FSPanel mode which adds new bezier line.
 * @author shunji
 *
 */
public class BezierDrawLineMode implements Mode {
	private final FSPanel panel;
	
	private BezierLineElementModel model;
	
	private NumberedColor color;
	
	private Pen pen;
	
	public BezierDrawLineMode(FSPanel panel, NumberedColor color, Pen pen) {
		this.panel = panel;
		this.color = color;
		this.pen = pen;
	}
	
	@Override
	public void mouseClicked(MouseEvent e) {}

	@Override
	public void mouseDragged(MouseEvent e) {
		model.addNewPoint(e.getPoint());
	}

	@Override
	public void mouseEntered(MouseEvent e) {}

	@Override
	public void mouseExited(MouseEvent e) {}

	@Override
	public void mouseMoved(MouseEvent e) {}

	@Override
	public void mousePressed(MouseEvent e) {
		model = new BezierLineElementModel();
		model.setColor(color);
		model.setPen(pen);
		model.addNewPoint(e.getPoint());
		LayerModel layerModel = panel.getLayerSelector().getActiveLayer();
		if(layerModel != null) {
			layerModel.addModel(model);
			panel.getElementByModel(model).setUpdating(true);
		}
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		model.lineFinished();
		Element element = panel.getElementByModel(model);
		if(e != null) {
			element.setUpdating(false);
		}
	}

	@Override
	public void modeFinished() {
	}

	@Override
	public void modeStarted() {
		panel.getSelectionModel().clear();
	}

	@Override
	public Cursor getMouseCursor() {
		return new Cursor(Cursor.DEFAULT_CURSOR);
	}

}
