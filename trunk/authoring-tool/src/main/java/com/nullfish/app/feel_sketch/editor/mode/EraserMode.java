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
import java.util.ArrayList;
import java.util.List;

import com.nullfish.app.feel_sketch.editor.Element;
import com.nullfish.app.feel_sketch.editor.Layer;
import com.nullfish.app.feel_sketch.editor.LayerModel;
import com.nullfish.app.feel_sketch.editor.Mode;
import com.nullfish.app.feel_sketch.editor.FSPanel;

/**
 * FSPanel mode todelete picture elements.
 * @author shunji
 *
 */
public class EraserMode implements Mode {
	private FSPanel panel;
	
	public EraserMode(FSPanel panel) {
		this.panel = panel;
	}
	
	@Override
	public void mouseClicked(MouseEvent e) {		
	}

	@Override
	public void mouseDragged(MouseEvent e) {
		LayerModel layerModel = panel.getLayerSelector().getActiveLayer();
		if(layerModel == null) {
			return;
		}
		List<Element> elements = new ArrayList<Element>(panel.getLayer(layerModel).getElements());
		for(Element element : elements) {
			if(element.contains(e.getPoint())) {
				layerModel.removeModel(element.getModel());
			}
		}
	}

	@Override
	public void mouseEntered(MouseEvent e) {}

	@Override
	public void mouseExited(MouseEvent e) {}

	@Override
	public void mouseMoved(MouseEvent e) {	}

	@Override
	public void mousePressed(MouseEvent e) {
		mouseMoved(e);
	}

	@Override
	public void mouseReleased(MouseEvent e) {
	}

	@Override
	public void modeFinished() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void modeStarted() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public Cursor getMouseCursor() {
		return new Cursor(Cursor.DEFAULT_CURSOR);
	}

}
