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

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Stroke;

import javax.swing.JPanel;
import javax.swing.JTable;
import javax.swing.table.TableCellRenderer;

@SuppressWarnings("serial")
public class BGSelectorTableCellRenderer extends JPanel implements
		TableCellRenderer {
	private Image image;
	
	private boolean isSelected;
	
	private Stroke selectionStroke = new BasicStroke(3);
	
	public static final int IMAGE_WIDTH = 80;
	
	public static final int IMAGE_HEIGHT = 60;
	
	public static final Color COLOR_SELECTED = Color.BLUE;
	
	public static final Color COLOR_NOT_SELECTED = Color.WHITE;
	
	public BGSelectorTableCellRenderer() {
		setOpaque(false);
	}

	@Override
	public Component getTableCellRendererComponent(JTable table, Object value,
			boolean isSelected, boolean hasFocus, int row, int column) {
		image = value != null ? BGImageThumbnailsCache.getInstance().getImage((Integer)value) : null;
		this.isSelected = isSelected;
		return this;
	}

	@Override
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		if(image == null) {
			return;
		}
		
		Graphics2D g2 = (Graphics2D) g
		;
		double width = image.getWidth(this);
		double height = image.getHeight(this);
		
		double wRatio = IMAGE_WIDTH / width;
		double hRatio = IMAGE_HEIGHT / height;
		
		double ratio = wRatio < hRatio ? wRatio : hRatio;
		ratio = ratio < 1 ? ratio : 1;
		
		g2.drawImage(image, (int)((getWidth() - (width * ratio)) / 2), (int)((getHeight() - (height * ratio)) / 2), (int)(width * ratio), (int)(height * ratio), this);
		
		if(isSelected) {
			g2.setColor(Color.BLUE);
			g2.setStroke(selectionStroke);
			g2.drawRect(3, 3, getWidth() - 6, getHeight() - 6);
		}
		
		
	}
}
