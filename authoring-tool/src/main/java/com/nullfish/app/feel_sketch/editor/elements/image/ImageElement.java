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

package com.nullfish.app.feel_sketch.editor.elements.image;

import java.awt.Composite;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Rectangle;
import java.awt.Shape;
import java.awt.geom.Point2D;

import com.nullfish.app.feel_sketch.editor.AbstractElement;
import com.nullfish.app.feel_sketch.editor.ImageCache;
import com.nullfish.app.feel_sketch.editor.Layer;
import com.nullfish.app.feel_sketch.editor.Locationable;

/**
 * An image element class.
 * @author shunji
 *
 */
public class ImageElement extends AbstractElement {
	private int width;
	
	private int height;
	
	public ImageElement(final ImageElementModel model, Layer layer) {
		super(model, layer);
		imageChanged();
	}

	@Override
	protected Shape getCurrentShape() {
		ImageElementModel model = (ImageElementModel) getModel();
		return new Rectangle(model.getLocation().x - (width / 2), model
				.getLocation().y
				- (height / 2), width, height);
	}

	@Override
	public void paint(Graphics2D g) {
		
		ImageElementModel model = (ImageElementModel) getModel();
		g.drawImage(ImageCache.getInstance().getImage(model.getImageId()),
				model.getLocation().x - (width / 2), model.getLocation().y
						- (height / 2), width, height, getPanel());
	}

	@Override
	public void modelAdded(String name, Object added) {
	}

	@Override
	public void modelChanged(String name, Object oldValue, Object newValue) {
		ImageElementModel model = (ImageElementModel) getModel();
		if(name.equals(ImageElementModel.IMAGE_ID)) {
			imageChanged();
		} else if(name.equals(Locationable.LOCATION)) {
			Point2D oldPoint = (Point2D) oldValue;
			Point2D newPoint = (Point2D) newValue;
			
			if(oldPoint != null) {
				addDirtyRegion(new Rectangle(
						(int)oldPoint.getX() - (width / 2), 
						(int)oldPoint.getY() - (height / 2), 
						width, height));
			}
			addDirtyRegion(new Rectangle(
					(int)newPoint.getX() - (width / 2), 
					(int)newPoint.getY() - (height / 2), 
					width, height));
			clearShape();
		}
	}
	
	private void imageChanged() {
		Image image = ImageCache.getInstance().getImage(((ImageElementModel)getModel()).getImageId());
		width = image.getWidth(getPanel());
		height = image.getHeight(getPanel());
		update();
	}

	@Override
	public void modelRemoved(String name, Object removed) {
	}
}
