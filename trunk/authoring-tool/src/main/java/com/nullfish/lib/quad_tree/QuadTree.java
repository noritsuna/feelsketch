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

package com.nullfish.lib.quad_tree;

import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class QuadTree<T> {
	private int maxLevel;

	private int width;

	private int height;

	private Cell<T>[] cells;

	private Map<T, Cell<T>> objCellMap = new HashMap<T, Cell<T>>();
	
	@SuppressWarnings("unchecked")
	public QuadTree(int maxLevel, int width, int height) {
		this.maxLevel = maxLevel;
		this.width = width;
		this.height = height;
		
		cells = new Cell[getBaseNumber(maxLevel + 1)];
		for(int i=0; i<cells.length; i++) {
			cells[i] = new Cell<T>();
		}
	}

	public List<T> getObjects(Rectangle2D rect) {
		CellInfo cellInfo = getCellInfo(rect);
		return getObjects(cellInfo.getLevel(), cellInfo.getId(), true);
	}
	
	public List<T> getObjects(int level, int id, boolean checkParent) {
		List<T> rtn = new ArrayList<T>();
		
		Cell<T> cell = cells[getBaseNumber(level) + id];
		if(cell.getItems() != null) {
			rtn.addAll(cell.getItems());
		}
		
		if(level < maxLevel) {
			for(int i=0; i<4; i++) {
				List<T> objs = getObjects(level + 1, (id << 2) + i, false);
				if(objs != null) {
					rtn.addAll(objs);
				}
			}
		}
		
		if(checkParent) {
			for(int i=0; i < level; i++) {
				cell = cells[getBaseNumber(level - i - 1) + (id >> ((i + 1) * 2))];
				if(cell.getItems() != null) {
					rtn.addAll(cell.getItems());
				}
			}
		}
		
		return rtn;
	}
	
	public void move(T o, Rectangle2D rect) {
		remove(o);
		
		Cell<T> newCell = getCell(rect);
		newCell.add(o);
		objCellMap.put(o, newCell);
	}
	
	public void remove(T o) {
		Cell<T> cell = objCellMap.get(o);
		if(cell != null) {
			cell.remove(o);
		}
	}
	
	private int get2DMortonNumber(Point2D point) {
		adjustPoint(point);
		double cellWidth = width / (Math.pow(2, maxLevel));
		double cellHeight = height / (Math.pow(2, maxLevel));
		return get2DMortonNumberFromCell((int)(point.getX() / cellWidth), (int)(point.getY() / cellHeight));
	}

	private int get2DMortonNumberFromCell(int x, int y) {
		return (bitSeparate32(x) | (bitSeparate32(y)<<1));
	}

	private int bitSeparate32(int n) {
		n = (n | (n << 8)) & 0x00ff00ff;
		n = (n | (n << 4)) & 0x0f0f0f0f;
		n = (n | (n << 2)) & 0x33333333;
		return (n | (n << 1)) & 0x55555555;
	}
	
	private CellInfo getCellInfo(Rectangle2D rect) {
		if(rect == null) {
			return new CellInfo(0, 0);
		}
		int topLeft = get2DMortonNumber(new Point2D.Double(rect.getX(), rect.getY()));
		int bottomRight = get2DMortonNumber(new Point2D.Double(rect.getMaxX(), rect.getMaxY()));
		
		return getCellInfo(topLeft, bottomRight);
		
	}

	private CellInfo getCellInfo(int topLeft, int bottomRight) {
		int shared = topLeft ^ bottomRight;
		
		int level = maxLevel;
		int id = topLeft;
		for(int i=0; i<maxLevel; i++) {
			if((shared & 3) != 0) {
				level = maxLevel - i - 1;
				id = topLeft >> (i * 2 + 2);
			}
			shared = shared >> 2;
		}
		
		return new CellInfo(level, id);
	}

	private Cell<T> getCell(Rectangle2D rect) {
		int topLeft = get2DMortonNumber(new Point2D.Double(rect.getX(), rect.getY()));
		int bottomRight = get2DMortonNumber(new Point2D.Double(rect.getMaxX(), rect.getMaxY()));
		
		return getCell(topLeft, bottomRight);
		
	}

	private Cell<T> getCell(int topLeft, int bottomRight) {
		int shared = topLeft ^ bottomRight;
		
		int level = maxLevel;
		int id = topLeft;
		for(int i=0; i<maxLevel; i++) {
			if((shared & 3) != 0) {
				level = maxLevel - i - 1;
				id = topLeft >> (i * 2 + 2);
			}
			shared = shared >> 2;
		}
		
		return cells[getBaseNumber(level) +id];
	}

	private void adjustPoint(Point2D p) {
		if(p.getX() < 0) {
			p.setLocation(0, p.getY());
		}
		if(p.getY() < 0) {
			p.setLocation(p.getX(), 0);
		}
		if(p.getX() >= width) {
			p.setLocation(width -1, p.getY());
		}
		if(p.getY() >= height) {
			p.setLocation(p.getX(), height - 1);
		}
	}

	private static int getBaseNumber(int level) {
		return ((int)(Math.pow(4, level)) - 1) / 3;
	}
}
