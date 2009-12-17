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

package com.nullfish.app.feel_sketch.ui.palette;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import com.nullfish.app.feel_sketch.editor.ImageCache;
import com.nullfish.app.feel_sketch.editor.mode.AddIconMode;

/**
 * A icon selecting palette class.
 * @author shunji
 *
 */
@SuppressWarnings("serial")
public class IconPalette extends JPanel {
	private Palette palette;
	
	private JPanel innerPanel = new JPanel(new GridLayout(IMAGE_NUMBER / H_BUTTONS_NUMBER + (IMAGE_NUMBER % H_BUTTONS_NUMBER != 0 ? 1 : 0), H_BUTTONS_NUMBER));
	private JScrollPane scroll = new JScrollPane(innerPanel);
	
	public static final int H_BUTTONS_NUMBER = 2;
	
	public static final int IMAGE_NUMBER = 15;
	
	public IconPalette(Palette palette) {
		super(new BorderLayout());
		this.palette = palette;
		initGui();
	}
	
	private void initGui() {
		scroll.setPreferredSize(new Dimension(140, 0));
		scroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		scroll.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		for(int i=1; i<=IMAGE_NUMBER; i++) {
			Image thumbnail = ImageCache.getInstance().getThumbnails(i);
			final int imageId = i;
			AbstractAction action = new AbstractAction() {
				
				@Override
				public void actionPerformed(ActionEvent e) {
					palette.getOwner().setMode(new AddIconMode(palette.getOwner(), imageId));
				}
			};
			JButton button = new JButton(action);
			button.setIcon(new ImageIcon(thumbnail));
			button.setPreferredSize(new Dimension(60, 60));
			
			innerPanel.add(button);
		}
		this.add(scroll);
	}
	
}
