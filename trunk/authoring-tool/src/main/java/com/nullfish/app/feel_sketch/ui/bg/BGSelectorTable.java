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

import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;

@SuppressWarnings("serial")
public class BGSelectorTable extends JTable {
	private BGSelectorTableModel model = new BGSelectorTableModel();
	
	public static final int COLUMN_WIDTH = 90;
	
	public static final int ROW_HEIGHT = 70;
	
	public BGSelectorTable() {
		setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
		getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		getColumnModel().getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		
		setDefaultRenderer(Object.class, new BGSelectorTableCellRenderer());
		setRowHeight(ROW_HEIGHT);
		setModel(model);
		
		model.addTableModelListener(new TableModelListener() {
			@Override
			public void tableChanged(TableModelEvent e) {
				BGSelectorTable.this.tableChanged();
			}
		});
		tableChanged();
		
		setColumnSelectionAllowed(true);
		
		getSelectionModel().addListSelectionListener(new ListSelectionListener() {
			
			@Override
			public void valueChanged(ListSelectionEvent e) {
				adjustColumn();
			}
		});

		getColumnModel().getSelectionModel().addListSelectionListener(new ListSelectionListener() {
			@Override
			public void valueChanged(ListSelectionEvent e) {
				adjustColumn();
			}
		});
	}
	
	private void adjustColumn() {
		ListSelectionModel columnSelectionModel = getColumnModel().getSelectionModel();
		int column = columnSelectionModel.getMinSelectionIndex();
		if(column <= 0) {
			return;
		}
		if(model.getValueAt(getSelectedRow(), column) == null) {
			columnSelectionModel.setSelectionInterval(column - 1, column - 1);
		}
	}
	
	public int getSelectedImageId() {
		if(getSelectedRow() < 0 || getSelectedColumn() < 0) {
			return -1;
		}
		return (Integer)model.getValueAt(getSelectedRow(), getSelectedColumn());
	}
	
	private void tableChanged() {
		for(int i=0; i<getColumnCount(); i++) {
			getColumnModel().getColumn(i).setPreferredWidth(COLUMN_WIDTH);
		}
	}
	
	public void selectImage(int imageId) {
		int index = model.getIndexOf(imageId);
		int x = index % model.getColumnCount();
		int y = index / model.getColumnCount();
		
		getSelectionModel().setSelectionInterval(y, y);
		getColumnModel().getSelectionModel().setSelectionInterval(x, x);
	}
}
