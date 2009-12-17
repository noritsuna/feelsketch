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

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

/**
 * A model class of a layer. 
 * @author shunji
 *
 */
public class LayerModel implements Serializable {
	
	private String name;
	
	private boolean toBePMCode = true;
	
	private List<ElementModel> models = new ArrayList<ElementModel>();
	
	private transient List<LayerModelListener> listeners = new ArrayList<LayerModelListener>();
	
	private int effect = 0;
	
	public LayerModel() {
	}
	
	public void addModel(ElementModel model) {
		models.add(model);
		initListenersIfNecessary();
		for(LayerModelListener listener : listeners) {
			listener.modelAdded(model);
		}
	}
	
	public void removeModel(ElementModel model) {
		models.remove(model);
		initListenersIfNecessary();
		for(LayerModelListener listener : listeners) {
			listener.modelRemoved(model);
		}
	}
	
	public boolean isToBePMCode() {
		return toBePMCode;
	}

	public void setToBePMCode(boolean toBePMCode) {
		this.toBePMCode = toBePMCode;
		initListenersIfNecessary();
		for(LayerModelListener listener : listeners) {
			listener.toBePMCodedChanged();
		}
	}

	public List<ElementModel> getModels() {
		return models;
	}
	
	public void addListener(LayerModelListener listener) {
		initListenersIfNecessary();
		listeners.add(listener);
	}
	
	public void removeListener(LayerModelListener listener) {
		initListenersIfNecessary();
		listeners.remove(listener);
	}

	private void initListenersIfNecessary() {
		if(listeners == null) {
			listeners = new ArrayList<LayerModelListener>();
		}
	}
	
	public int getEffect() {
		return effect;
	}

	public void setEffect(int effect) {
		this.effect = effect;
		for(LayerModelListener listener : listeners) {
			listener.effectChanged();
		}
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
		for(LayerModelListener listener : listeners) {
			listener.nameChanged();
		}
	}

}
