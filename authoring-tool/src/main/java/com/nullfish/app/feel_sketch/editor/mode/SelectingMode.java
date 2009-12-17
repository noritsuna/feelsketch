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
import java.awt.Point;
import java.awt.event.MouseEvent;
import java.util.ArrayList;

import com.nullfish.app.feel_sketch.editor.Element;
import com.nullfish.app.feel_sketch.editor.Locationable;
import com.nullfish.app.feel_sketch.editor.Mode;
import com.nullfish.app.feel_sketch.editor.FSPanel;
import com.nullfish.app.feel_sketch.editor.SelectionModel;

/**
 * FSPanel mode to select picture elements.
 * @author shunji
 *
 */
public class SelectingMode implements Mode {
	private FSPanel panel;
	
	private Point point;
	
	public SelectingMode(FSPanel panel) {
		this.panel = panel;
	}

	@Override
	public void modeFinished() {
	}

	@Override
	public void modeStarted() {
	}

	@Override
	public void mouseClicked(MouseEvent e) {
	}

	@Override
	public void mouseDragged(MouseEvent e) {
		Element element = panel.getElementAtPointOnActiveLayer(point);
		if(element != null && 
				panel.getSelectionModel().isSelected(element) && 
				element.getModel() instanceof Locationable) {
			panel.setMode(new DragMode(panel, new ArrayList<Element>(panel.getSelectionModel().getSelectedElements()), point));
		} else {
			panel.setMode(new DraggingSelectingMode(panel, point));
		}
	}

	@Override
	public void mouseEntered(MouseEvent e) {
		// TODO Auto-generated method stub

	}

	@Override
	public void mouseExited(MouseEvent e) {
		// TODO Auto-generated method stub

	}

	@Override
	public void mouseMoved(MouseEvent e) {
		// TODO Auto-generated method stub

	}

	@Override
	public void mousePressed(MouseEvent e) {
		point = e.getPoint();

		SelectionModel selectionModel = panel.getSelectionModel();

		Element element = panel.getElementAtPointOnActiveLayer(e.getPoint());
		if(element == null) {
			if(!e.isControlDown()) {
				selectionModel.clear();
			}
			return;
		}
		if(e.isControlDown()) {
			selectionModel.setElementSelected(element, !selectionModel.isSelected(element));
		} else {
			if(!selectionModel.isSelected(element)) {
				selectionModel.clear();
			}
			selectionModel.addSelectedElement(element);
		}
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		// TODO Auto-generated method stub

	}

	@Override
	public Cursor getMouseCursor() {
		return new Cursor(Cursor.DEFAULT_CURSOR);
	}

}
