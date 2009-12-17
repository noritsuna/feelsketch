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
import java.awt.Rectangle;
import java.awt.event.MouseEvent;
import java.util.HashSet;
import java.util.Set;

import com.nullfish.app.feel_sketch.editor.Element;
import com.nullfish.app.feel_sketch.editor.Layer;
import com.nullfish.app.feel_sketch.editor.LayerModel;
import com.nullfish.app.feel_sketch.editor.Mode;
import com.nullfish.app.feel_sketch.editor.FSPanel;

/**
 * FSPanel mode of area selection. 
 * @author shunji
 *
 */
public class DraggingSelectingMode implements Mode {
	private FSPanel panel;
	
	private Point start;
	private Point end;
	
	private Set<Element> orgSelectedElements = new HashSet<Element>();
	private Set<Element> newSelectedElements = new HashSet<Element>();
	
	public DraggingSelectingMode(FSPanel panel, Point start) {
		this.panel = panel;
		this.start = start;
		orgSelectedElements.addAll(panel.getSelectionModel().getSelectedElements());
	}
	
	@Override
	public void modeFinished() {
		panel.setSelectionRectangle(null);
	}

	@Override
	public void modeStarted() {
	}

	@Override
	public void mouseClicked(MouseEvent e) {
	}

	@Override
	public void mouseDragged(MouseEvent e) {
		end = e.getPoint();
		
		Rectangle rect = new Rectangle(start);
		rect.add(end);
		panel.setSelectionRectangle(rect);

		Set<Element> notSelected = new HashSet<Element>();
		for(Element element : newSelectedElements) {
			if(!element.intersects(rect)) {
				notSelected.add(element);
			}
		}

		for(Element element : notSelected) {
			panel.getSelectionModel().removeSelecetedElement(element);
			newSelectedElements.remove(element);
		}
		
		LayerModel activeLayerModel = panel.getLayerSelector().getActiveLayer();
		if(activeLayerModel != null) {
			for(Element element : panel.getLayer(activeLayerModel).getElements()) {
				if(element.intersects(rect)) {
					panel.getSelectionModel().addSelectedElement(element);
					
					if(!orgSelectedElements.contains(element)) {
						newSelectedElements.add(element);
					}
				}
			}
		}
	}

	@Override
	public void mouseEntered(MouseEvent e) {
	}

	@Override
	public void mouseExited(MouseEvent e) {
	}

	@Override
	public void mouseMoved(MouseEvent e) {
	}

	@Override
	public void mousePressed(MouseEvent e) {
		start = e.getPoint();
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		panel.setMode(new SelectingMode(panel));
	}

	@Override
	public Cursor getMouseCursor() {
		return new Cursor(Cursor.DEFAULT_CURSOR);
	}

}
