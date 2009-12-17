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

import java.awt.Rectangle;
import java.awt.Shape;
import java.awt.geom.Point2D;

import javax.swing.RepaintManager;

/**
 * An abstract class of the pictures elements.
 * @author shunji
 *
 */
public abstract class AbstractElement implements Element {
	private ElementModel model;
	
	private Layer layer;
	
	private Shape shape;

	/**
	 * The showing order
	 */
	private int order = orderSeed++;
	
	private static int orderSeed = 0;

	/**
	 * This element will be repainted anytime if it is true.
	 */
	private boolean updating;
	
	/**
	 * The constructor
	 * @param model
	 * @param layer
	 */
	public AbstractElement(ElementModel model, Layer layer) {
		this.model = model;
		this.layer = layer;
		model.addListener(new ElementModelListener() {
			
			@Override
			public void added(String name, Object added) {
				modelAdded(name, added);
			}

			@Override
			public void changed(String name, Object oldValue, Object newValue) {
				modelChanged(name, oldValue, newValue);
			}

			@Override
			public void removed(String name, Object removed) {
				modelRemoved(name, removed);
			}
		});
	}
	
	public abstract void modelAdded(String name, Object added);

	public abstract void modelChanged(String name, Object oldValue, Object newValue);

	public abstract void modelRemoved(String name, Object removed);
	
	@Override
	public ElementModel getModel() {
		return model;
	}

	public void setModel(ElementModel model) {
		this.model = model;
	}

	@Override
	public boolean contains(Point2D p) {
		return getShape().contains(p);
	}

	@Override
	public boolean intersects(Shape shape) {
		return getShape().intersects(shape.getBounds2D());
	}
	
	@Override
	public Shape getShape() {
		if(shape == null) {
			shape = getCurrentShape();
			layer.getQuadTree().move(this, shape.getBounds2D());
		}
		return shape;
	}
	
	protected abstract Shape getCurrentShape();

	protected void clearShape() {
		shape = null;
	}
	
	public void update() {
		Rectangle rect = getShape().getBounds();
		RepaintManager.currentManager(layer.getPanel()).addDirtyRegion(layer.getPanel(), rect.x, rect.y, rect.width, rect.height);
	}
	
	public FSPanel getPanel() {
		return layer.getPanel();
	}

	protected void addDirtyRegion(Rectangle rect) {
		RepaintManager.currentManager(getPanel()).addDirtyRegion(
				getPanel(), rect.x, rect.y, rect.width, rect.height);
	}

	@Override
	public int getOrder() {
		return order;
	}

	@Override
	public boolean isUpdating() {
		return updating;
	}
	
	@Override
	public void setUpdating(boolean updating) {
		this.updating = updating;
		if(updating) {
			layer.addUpdatingElement(this);
		} else {
			layer.removeUpdatingElement(this);
		}
	}
}
