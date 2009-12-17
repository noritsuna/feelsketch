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

import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.DefaultListCellRenderer;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.plaf.DimensionUIResource;

import com.nullfish.app.feel_sketch.editor.NumberedColor;
import com.nullfish.app.feel_sketch.editor.Pen;
import com.nullfish.app.feel_sketch.editor.mode.BezierDrawLineMode;
import com.nullfish.app.feel_sketch.ui.PenImageFactory;

@SuppressWarnings("serial")
public class PenPalette extends JPanel {
	private Pen pen = Pen.narrow;
	private NumberedColor penColor = NumberedColor.BLACK;
	
	
	private Palette palette;
	private JComboBox penWidthComboBox = new JComboBox();
	
	private JPanel penPanel = new JPanel();
	
	private PenImageFactory penImageFactory = new PenImageFactory();
	
	public PenPalette(Palette palette) {
		this.palette = palette;
		initGui();
	}
	
	private void initGui() {
		setLayout(new GridBagLayout());
		
		JPanel topPanel = new JPanel();
		topPanel.setLayout(new BoxLayout(topPanel, BoxLayout.X_AXIS));
		add(topPanel, new GridBagConstraints(0, 0, 1, 1, 1.0, 0.0, GridBagConstraints.WEST, GridBagConstraints.BOTH, new Insets(2, 2, 2, 2), 0, 0));
		penPanel.setOpaque(true);
		penPanel.setPreferredSize(new Dimension(30, 30));
		penPanel.setBorder(BorderFactory.createEtchedBorder());
		topPanel.add(penPanel);
		
		penWidthComboBox.addItem(Pen.narrow);
		penWidthComboBox.addItem(Pen.midium);
		penWidthComboBox.addItem(Pen.wide);
		penWidthComboBox.addItem(Pen.veryWide);
		penWidthComboBox.addItem(Pen.highliterNarrow);
		penWidthComboBox.addItem(Pen.highliterMidium);
		penWidthComboBox.addItem(Pen.highliterWide);
		
		penWidthComboBox.setSelectedItem(pen);
		penWidthComboBox.setRenderer(new DefaultListCellRenderer() {
			
			@Override
			public Component getListCellRendererComponent(JList list, Object value,
					int index, boolean isSelected, boolean cellHasFocus) {
				setIcon(new ImageIcon(penImageFactory.getImage((Pen)value)));
				return this;
			}
		});

		penWidthComboBox.addItemListener(new ItemListener() {
			
			@Override
			public void itemStateChanged(ItemEvent e) {
				setPen((Pen)penWidthComboBox.getSelectedItem());
			}
		});
		
		topPanel.add(penWidthComboBox);
		
		JPanel colorButtonsPanel = new JPanel(new GridLayout(6, 3, 2, 2));
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(1)));
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(2)));
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(3)));
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(4)));
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(5)));
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(6)));
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(7)));
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(8)));
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(9)));
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(10)));
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(11)));
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(12)));
/*
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(32)));
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(33)));
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(34)));
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(35)));
		colorButtonsPanel.add(new ColorButton(NumberedColor.getColor(36)));
*/
		penPanel.setBackground(penColor);
		add(colorButtonsPanel, new GridBagConstraints(0, 1, 1, 1, 1.0, 0.0, GridBagConstraints.WEST, GridBagConstraints.BOTH, new Insets(2, 2, 2, 2), 0, 0));
		
		add(new JPanel(), new GridBagConstraints(0, 2, 1, 1, 1.0, 1.0, GridBagConstraints.WEST, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));
		
	}
	
	public void init() {
		setColor(penColor);
		penWidthComboBox.setSelectedItem(pen);
	}
	
	public void startPenMode() {
		palette.getOwner().setMode(new BezierDrawLineMode(palette.getOwner(), penColor, pen));
	}
	
	private class ColorButton extends JPanel {
		ColorButton(final NumberedColor color) {
			setPreferredSize(new DimensionUIResource(30, 30));
			setBorder(BorderFactory.createEtchedBorder());
			setOpaque(true);
			setBackground(color);
			addMouseListener(new MouseAdapter() {
				@Override
				public void mouseClicked(MouseEvent e) {
					setColor(color);
				}
			});
		}
	}
	
	@SuppressWarnings("unused")
	private class PenWidthButuon extends JButton {
		PenWidthButuon(final Pen pen) {
			setIcon(new ImageIcon(penImageFactory.getImage(pen)));
			setBorder(BorderFactory.createEtchedBorder());
			
			addActionListener(new ActionListener() {
				
				@Override
				public void actionPerformed(ActionEvent e) {
					setPen(pen);
				}
			});
		}
	}
	
	private void setColor(NumberedColor color) {
		penColor = color;
		penPanel.setBackground(color);
		palette.startPenMode();
	}
	
	private void setPen(Pen pen) {
		this.pen = pen;
		palette.startPenMode();
	}
	
	
}
