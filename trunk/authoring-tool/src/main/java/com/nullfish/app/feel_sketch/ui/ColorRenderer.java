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

package com.nullfish.app.feel_sketch.ui;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;

import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.ListCellRenderer;

import com.nullfish.app.feel_sketch.editor.NumberedColor;

public class ColorRenderer extends JPanel implements ListCellRenderer {
	private NumberedColor color;
	
	public ColorRenderer() {
		setPreferredSize(new Dimension(50, 20));
	}
	
    @Override
    public Component getListCellRendererComponent(JList list,
            Object object, int index, boolean isSelected,
            boolean hasFocus) {
        color = (NumberedColor)object;
        return this;
    }
    
    @Override
    protected void paintComponent(Graphics g) {
    	super.paintComponent(g);
    	
    	int width = getWidth();
    	g.setColor(color);
    	g.fillRect(5, 2, width - 10, 16);
    }
}
