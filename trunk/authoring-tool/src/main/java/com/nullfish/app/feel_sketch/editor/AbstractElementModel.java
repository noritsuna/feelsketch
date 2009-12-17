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
 * An model class of picture elements.
 * @author shunji
 *
 */
public abstract class AbstractElementModel implements ElementModel, Serializable {
	
	private transient List<ElementModelListener> listeners = new ArrayList<ElementModelListener>(); 
	
	@Override
	public void addListener(ElementModelListener listener) {
		initListenersIfNecessary();
		listeners.add(listener);
	}

	@Override
	public void clearListener() {
		initListenersIfNecessary();
		listeners.clear();
	}

	@Override
	public void removeListener(ElementModelListener listener) {
		initListenersIfNecessary();
		listeners.remove(listener);
	}

	protected void notifyChange(String name, Object oldValue, Object newValue) {
		initListenersIfNecessary();
		if(newValue == oldValue
			|| (newValue == null && oldValue == null)) {
			return;
		}
		
		if((newValue == null || oldValue == null) ||
			!newValue.equals(oldValue)) {
			for(ElementModelListener listener : listeners) {
				listener.changed(name, oldValue, newValue);
			}
		}
	}

	protected void notifyAdded(String name, Object value) {
		initListenersIfNecessary();
		for(ElementModelListener listener : listeners) {
			listener.added(name, value);
		}
	}

	protected void notifyRemoved(String name, Object value) {
		initListenersIfNecessary();
		for(ElementModelListener listener : listeners) {
			listener.removed(name, value);
		}
	}
	
	private void initListenersIfNecessary() {
		if(listeners == null) {
			listeners = new ArrayList<ElementModelListener>();
		}
	}
	
	public boolean isToBePMCode() {
		// TODO:
		return true;
	}
}
