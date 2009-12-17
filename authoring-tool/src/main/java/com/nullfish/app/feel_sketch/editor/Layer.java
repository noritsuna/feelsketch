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

import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Rectangle;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.nullfish.lib.quad_tree.QuadTree;

public class Layer {
	private LayerModel model;
	
	private FSPanel panel;
	
	private List<Element> elements = new ArrayList<Element>();
	
	private Map<ElementModel, Element> modelElementMap = new HashMap<ElementModel, Element>();

	private QuadTree<Element> quadTree;
	
	private Set<Element> updatingElements = new HashSet<Element>();
	
	private Comparator<Element> orderComparator = new Comparator<Element>() {
		
		@Override
		public int compare(Element o1, Element o2) {
			return o1.getOrder() - o2.getOrder();
		}
	};
	
	public Layer(final FSPanel panel, LayerModel model) {
		this.panel = panel;
		this.model = model;
		
		quadTree = new QuadTree<Element>(4, panel.getWidth(), panel.getHeight());
		
		for(ElementModel elementModel : model.getModels()) {
			modelAdded(elementModel);
		}
		
		model.addListener(new LayerModelListener() {
			
			@Override
			public void toBePMCodedChanged() {
			}
			
			@Override
			public void modelRemoved(ElementModel model) {
				Layer.this.modelRemoved(model);
			}
			
			@Override
			public void modelAdded(ElementModel model) {
				Layer.this.modelAdded(model);
			}

			@Override
			public void effectChanged() {
			}

			@Override
			public void nameChanged() {
			}
		});
	}
	
	public LayerModel getModel() {
		return model;
	}

	private void modelRemoved(ElementModel model) {
		Element element = modelElementMap.get(model);
		elements.remove(element);
		quadTree.remove(element);
		panel.getSelectionModel().removeSelecetedElement(element);
		element.update();
	}
	
	private void modelAdded(ElementModel model) {
		Element element = ElementFactory.createElement(model, this);
		elements.add(element);
		modelElementMap.put(model, element);
		quadTree.move(element, element.getShape().getBounds2D());
		element.update();
	}
	
	public void paint(Graphics2D g) {
		Rectangle clip = g.getClipBounds();
		List<Element> elements = quadTree.getObjects(clip);
		for(Element e : updatingElements) {
			if(!elements.contains(e)) {
				elements.add(e);
			}
		}
		Collections.sort(elements, orderComparator);
		for(Element element : elements) {
			if(clip == null || element.isUpdating() || element.intersects(clip)) {
				element.paint(g);
			}
		}
	}
	
	public List<Element> getElements() {
		return elements;
	}
	
	public Element getElementByModel(ElementModel model) { 
		return modelElementMap.get(model);
	}
	
	public Element getElementAtPoint(Point p) {
		for(int i=0; i<elements.size(); i++) {
			Element element = elements.get(elements.size() - i - 1);
			if(element.getShape().contains(p)) {
				return element;
			}
		}
		return null;
	}
	
	public QuadTree<Element> getQuadTree() {
		return quadTree;
	}

	public FSPanel getPanel() {
		return panel;
	}

	public void addUpdatingElement(Element element) {
		updatingElements.add(element);
	}
	
	public void removeUpdatingElement(Element element) {
		updatingElements.remove(element);
	}
	
}
