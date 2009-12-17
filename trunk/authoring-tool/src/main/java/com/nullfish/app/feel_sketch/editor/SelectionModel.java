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

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

/**
 * A model class of picture elements selection.
 * @author shunji
 *
 */
public class SelectionModel {
	private List<SelectionModelListener> listeners = new ArrayList<SelectionModelListener>();
	
	private Set<Element> selectedElements = new HashSet<Element>();
	
	public void addSelectedElement(Element element) {
		if(selectedElements.contains(element)) {
			return;
		}
		selectedElements.add(element);
		notifySelectionChanged(element);
	}
	
	public void removeSelecetedElement(Element element) {
		if(!selectedElements.contains(element)) {
			return;
		}
		selectedElements.remove(element);
		notifySelectionChanged(element);
	}
	
	public void clear() {
		Set<Element> old = selectedElements;
		selectedElements = new HashSet<Element>();
		for(Element e : old) {
			notifySelectionChanged(e);
		}
	}
	
	public boolean isSelected(Element element) {
		return selectedElements.contains(element);
	}
	
	public void setElementSelected(Element element, boolean selected) {
		boolean orgValue = selectedElements.contains(element);
		if(orgValue == selected) {
			return;
		}
		
		if(selected) {
			selectedElements.add(element);
		} else {
			selectedElements.remove(element);
		}
		
		notifySelectionChanged(element);
	}
	
	public Set<Element> getSelectedElements() {
		return selectedElements;
	}
	
	public void addListener(SelectionModelListener listener) {
		listeners.add(listener);
	}

	public void removeListener(SelectionModelListener listener) {
		listeners.remove(listener);
	}
	
	private void notifySelectionChanged(Element e) {
		for(SelectionModelListener listener : listeners) {
			listener.selectionChanged(e);
		}
	}

}
