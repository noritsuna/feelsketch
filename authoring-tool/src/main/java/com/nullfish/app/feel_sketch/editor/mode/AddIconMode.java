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

import com.nullfish.app.feel_sketch.editor.FSPanel;
import com.nullfish.app.feel_sketch.editor.LayerModel;
import com.nullfish.app.feel_sketch.editor.Mode;
import com.nullfish.app.feel_sketch.editor.elements.image.ImageElementModel;

/**
 * FSPanel mode which adds icons.
 * @author shunji
 *
 */
public class AddIconMode implements Mode {
	private FSPanel panel;
	
	private int imageId;
	
	public AddIconMode(FSPanel panel, int imageId) {
		this.panel = panel;
		this.imageId = imageId;
	}
	
	@Override
	public void modeFinished() {
	}

	@Override
	public void modeStarted() {
		panel.getSelectionModel().clear();
	}

	@Override
	public void mouseClicked(MouseEvent e) {
	}

	@Override
	public void mouseDragged(MouseEvent e) {
	}

	@Override
	public void mouseEntered(MouseEvent e) {
	}

	@Override
	public void mouseExited(MouseEvent e) {
	}

	@Override
	public void mouseMoved(MouseEvent e) {
		// TODO Auto-generated method stub

	}

	@Override
	public void mousePressed(MouseEvent e) {
		ImageElementModel model = new ImageElementModel(imageId);
		model.setLocation(e.getPoint());
		LayerModel layerModel = panel.getLayerSelector().getActiveLayer();
		if(layerModel != null) {
			layerModel.addModel(model);
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
