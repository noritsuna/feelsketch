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

import java.awt.Dimension;
import java.awt.Point;

import com.nullfish.app.feel_sketch.ui.FSResource;

/**
 * Enumeration of the position of PM code in an image.
 * @author shunji
 *
 */
public enum PMCodePosition {
	topLeft(1, "topLeft"), 
	centerLeft(2, "centerLeft"), 
	bottomLeft(3, "bottomLeft"), 
	topRight(4, "topRight"), 
	centerRight(5, "centerRight"), 
	bottomRight(6, "bottomRight"), 
	topCenter(7, "topCenter"), 
	centerCenter(7, "centerCenter"), 
	bottomCenter(9, "bottomCenter");

	private int value;
	
	private String label;
	
	private PMCodePosition(int value, String label) {
		this.value = value;
		this.label = label;	
	}
	
	public int getValue() {
		return value;
	}

	public String getLabel() {
		return label;
	}

	public String toString() {
		return FSResource.getString(label);
	}
	
	public Point getPoint(Dimension orgSize, Dimension pmCodeSize) {
		if(this == topLeft) {
			return new Point();
		} else if(this == centerLeft) {
			return new Point(0, (orgSize.height - pmCodeSize.height) / 2);
		} else if(this == bottomLeft) {
			return new Point(0, orgSize.height - pmCodeSize.height);
		} else if(this == topRight) {
			return new Point(orgSize.width - pmCodeSize.width, 0);
		} else if(this == centerRight) {
			return new Point(orgSize.width - pmCodeSize.width, (orgSize.height - pmCodeSize.height) / 2) ;
		} else if(this == bottomRight) {
			return new Point(orgSize.width - pmCodeSize.width, orgSize.height - pmCodeSize.height) ;
		} else if(this == topCenter) {
			return new Point((orgSize.width - pmCodeSize.width) / 2, 0) ;
		} else if(this == centerCenter) {
			return new Point((orgSize.width - pmCodeSize.width) / 2, (orgSize.height - pmCodeSize.height) / 2) ;
		} else if(this == bottomCenter) {
			return new Point((orgSize.width - pmCodeSize.width) / 2, orgSize.height - pmCodeSize.height) ;
		}
		return null;
	}
}
