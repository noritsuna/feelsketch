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

package com.nullfish.app.feel_sketch.ui.bg;

import java.util.ArrayList;
import java.util.List;

import javax.swing.table.AbstractTableModel;

import com.nullfish.app.feel_sketch.util.ThreadSafeUtil;

/**
 * @author shunji
 *
 */
@SuppressWarnings("serial")
public class BGSelectorTableModel extends AbstractTableModel {
	private List<Integer> imageIds = new ArrayList<Integer>();
	
	private int columnCount = 5;
	
	/**
	 * The maximum image id.
	 */
	public static final int MAX_COUNT = 64;

	public static final int MAX_COLOR = 32;

	public BGSelectorTableModel() {
		BGImageThumbnailsCache.getInstance().addListener(new BGImageThumbnailsCacheListener() {
			@Override
			public void imageLoaded(int imageId) {
				BGSelectorTableModel.this.imageLoaded(imageId);
			}
		});
		
		for(int i=1; i<=MAX_COLOR; i++) {
			imageIds.add(i);
		}
		for(int i=32; i<=MAX_COUNT; i++) {
			imageIds.add(i);
		}
	}
	
	@Override
	public int getColumnCount() {
		return columnCount;
	}

	@Override
	public int getRowCount() {
		return imageIds.size() / columnCount + (imageIds.size() % columnCount != 0 ? 1 : 0);
	}

	@Override
	public Object getValueAt(int rowIndex, int columnIndex) {
		int index = rowIndex * columnCount + columnIndex;
		return index >= 0 && index < imageIds.size() ? imageIds.get(index) : null;
	}
	
	public void imageLoaded(final int id) {
		Runnable runnable = new Runnable() {
			
			@Override
			public void run() {
				fireTableCellUpdated(id / columnCount, id % columnCount);
			}
		};
		ThreadSafeUtil.executeRunnable(runnable);
	}
	
	public void setColumnCount(int columnCount) {
		this.columnCount = columnCount;
		fireTableStructureChanged();
	}
	
	public int getIndexOf(int imageId) {
		return imageIds.indexOf(imageId);
	}
}
