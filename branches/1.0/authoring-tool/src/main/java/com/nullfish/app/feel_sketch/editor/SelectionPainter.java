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

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.Shape;
import java.util.Map;
import java.util.WeakHashMap;

import javax.swing.RepaintManager;

/**
 * A class which draws selection of elements.
 * @author shunji
 *
 */
public class SelectionPainter {
	private SelectionModel selectionModel;
	
	private FSPanel panel;
	
	private Map<Element, Shape> elementShapeMap = new WeakHashMap<Element, Shape>();
	
	private FSModelListener samoyedModelListener = new FSModelListener() {
		
		@Override
		public void layerRemoved(LayerModel layerModel) {
			for(ElementModel model : layerModel.getModels()) {
				elementShapeMap.remove(panel.getElementByModel(model));
			}
		}
		
		@Override
		public void layerAdded(LayerModel layerModel) {
			SelectionPainter.this.layerAdded(layerModel);
		}
		
		
		@Override
		public void backgroundImageChanged() {
		}

		@Override
		public void sizeChanged() {
		}

		@Override
		public void layerOrderChanged() {
		}
	};
	
	public static final Color SELECTION_COLOR = new Color(0, 0, 128);
	public static final float[] SELECTION_STROKE_DASH = {3.0f, 3.0f};
	public static final BasicStroke SELECTION_STROKE = new BasicStroke(1.0f, 
                                           BasicStroke.CAP_BUTT, 
                                           BasicStroke.JOIN_MITER, 
                                           10.0f, 
                                           SELECTION_STROKE_DASH, 
                                           0.0f);
	
	public SelectionPainter(final FSPanel panel) {
		this.panel = panel;
		selectionModel = panel.getSelectionModel();
		selectionModel.addListener(new SelectionModelListener() {
			
			@Override
			public void selectionChanged(Element element) {
				Rectangle rect = SELECTION_STROKE.createStrokedShape(element.getShape()).getBounds();
				RepaintManager.currentManager(panel).addDirtyRegion(panel, rect.x, rect.y, rect.width, rect.height);
			}
		});
		init();
	}
	
	public void init() {
		panel.getModel().addListener(samoyedModelListener);
		for(LayerModel layerModel : panel.getModel().getLayersModel()) {
			layerAdded(layerModel);
		}
	}
	
	private void update(Element element) {
		Shape oldShape = elementShapeMap.remove(element);
		if(oldShape != null) {
			Rectangle oldRect = SELECTION_STROKE.createStrokedShape(oldShape).getBounds();
			RepaintManager.currentManager(panel).addDirtyRegion(panel, oldRect.x, oldRect.y, oldRect.width, oldRect.height);
		}
	}
	
	public void paint(Graphics2D g) {
		g.setColor(SELECTION_COLOR);
		g.setStroke(SELECTION_STROKE);
		for(Element element : selectionModel.getSelectedElements()) {
			Shape shape = elementShapeMap.get(element);
			if(shape == null) {
				shape = element.getShape();
				elementShapeMap.put(element, shape);
			}

			g.draw(shape);
		}
	}
	
	public void layerAdded(final LayerModel layerModel) {
		layerModel.addListener(new LayerModelListener() {
			
			@Override
			public void toBePMCodedChanged() {
			}
			
			@Override
			public void modelRemoved(ElementModel model) {
				elementShapeMap.remove(panel.getElementByModel(model));
			}
			
			@Override
			public void modelAdded(final ElementModel model) {
				model.addListener(new ElementModelListener() {
					
					@Override
					public void removed(String name, Object removed) {
						update(panel.getElementByModel(model));
					}
					
					@Override
					public void changed(String name, Object oldValue, Object newValue) {
						update(panel.getElementByModel(model));
					}
					
					@Override
					public void added(String name, Object added) {
						update(panel.getElementByModel(model));
					}
				});
			}

			@Override
			public void effectChanged() {
			}

			@Override
			public void nameChanged() {
			}
		});
		
		for(final ElementModel model : layerModel.getModels()) {
			model.addListener(new ElementModelListener() {
				
				@Override
				public void removed(String name, Object removed) {
					update(panel.getElementByModel(model));
				}
				
				@Override
				public void changed(String name, Object oldValue, Object newValue) {
					update(panel.getElementByModel(model));
				}
				
				@Override
				public void added(String name, Object added) {
					update(panel.getElementByModel(model));
				}
			});
			
		}
	}
}
