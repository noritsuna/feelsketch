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
import java.awt.CardLayout;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;

import javax.swing.JPanel;

import com.nullfish.app.feel_sketch.editor.FSPanel;
import com.nullfish.app.feel_sketch.editor.mode.EraserMode;
import com.nullfish.app.feel_sketch.editor.mode.SelectingMode;
import com.nullfish.lib.tablelayout.HtmlTablePanel;

/**
 * A palette class of Feel Sketch.
 * @author shunji
 *
 */
@SuppressWarnings("serial")
public class Palette extends JPanel {
	private FSPanel owner;
	
	private HtmlTablePanel panel;
	
	private CardLayout cardLayout = new CardLayout();
	private JPanel subPalettePanel = new JPanel(cardLayout);
	
	private PenPalette penPalette = new PenPalette(this);
	
	private IconPalette iconPalette = new IconPalette(this);
	
	private PrefPalette prefPalette = new PrefPalette(this);

	public static final String MODE_PEN = "bezier";
	public static final String MODE_ICON = "icon";
	public static final String MODE_NONE = "none";
	public static final String MODE_ERASER = MODE_NONE;
	public static final String MODE_SELECT = MODE_NONE;
	public static final String MODE_PREF = "pref";
	
	public Palette() {
		super(new BorderLayout());
		try {
			panel = new HtmlTablePanel("/com/nullfish/app/feel_sketch/ui/palette.xml");
			panel.layoutByMemberName(this);
			add(panel);
			
			subPalettePanel.add(new JPanel(), MODE_NONE);
			subPalettePanel.add(penPalette, MODE_PEN);
			subPalettePanel.add(iconPalette, MODE_ICON);
			subPalettePanel.add(prefPalette, MODE_PREF);
		} catch (Exception e) {
			e.printStackTrace();
		}

	}
	
	public FSPanel getOwner() {
		return owner;
	}
	
	public void setOwner(final FSPanel owner) {
		this.owner = owner;
		owner.addComponentListener(new ComponentAdapter() {
			@Override
			public void componentResized(ComponentEvent e) {
				prefPalette.applyModel(owner.getModel());
			}
		});
	}
	
	public void startPenMode() {
		cardLayout.show(subPalettePanel, MODE_PEN);
		penPalette.startPenMode();
	}

	public void startIconMode() {
		cardLayout.show(subPalettePanel, MODE_ICON);
	}

	public void startEraserMode() {
		cardLayout.show(subPalettePanel, MODE_ERASER);
		owner.setMode(new EraserMode(owner));
	}
	
	public void startSelectMode() {
		cardLayout.show(subPalettePanel, MODE_SELECT);
		owner.setMode(new SelectingMode(owner));
	}
	
	public void startPrefMode() {
		cardLayout.show(subPalettePanel, MODE_PREF);
	}

}
