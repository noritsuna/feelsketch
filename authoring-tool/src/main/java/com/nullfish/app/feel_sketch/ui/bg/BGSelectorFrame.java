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

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ButtonGroup;
import javax.swing.JFrame;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import com.nullfish.app.feel_sketch.editor.FSModel;
import com.nullfish.app.feel_sketch.editor.FSPanel;
import com.nullfish.app.feel_sketch.ui.FSResource;
import com.nullfish.lib.tablelayout.HtmlTablePanel;

@SuppressWarnings("serial")
public class BGSelectorFrame extends JFrame {
	private FSPanel owner;
	
	private BGSelectorTable selectorPanel = new BGSelectorTable();
	
	private JRadioButton bgEqualRadio = new JRadioButton(FSResource.getString("bg_center"));
	private JRadioButton bgTileRadio = new JRadioButton(FSResource.getString("bg_tile"));
	private JRadioButton bgMaximizeRadio = new JRadioButton(FSResource.getString("bg_max"));
	
	private ButtonGroup bgRadioGroup = new ButtonGroup();
	
	@SuppressWarnings("unused")
	private JScrollPane scroll = new JScrollPane(selectorPanel) {
		public void setColumnHeaderView(Component view) {
		}
	};
	
	public BGSelectorFrame(FSPanel owner) {
		this.owner = owner;
		
		initGui();
	}

	private void initGui() {
		setTitle(FSResource.getString("bg_select"));
		HtmlTablePanel panel;
		try {
			panel = new HtmlTablePanel("/com/nullfish/app/feel_sketch/ui/bg/layout.xml");
			panel.layoutByMemberName(this);
			getContentPane().add(panel);
			
			selectorPanel.getSelectionModel().addListSelectionListener(new ListSelectionListener() {
				
				@Override
				public void valueChanged(ListSelectionEvent e) {
					imageSelected();
				}
			});

			selectorPanel.getColumnModel().getSelectionModel().addListSelectionListener(new ListSelectionListener() {
				
				@Override
				public void valueChanged(ListSelectionEvent e) {
					imageSelected();
				}
			});
			
			bgRadioGroup.add(bgEqualRadio);
			bgRadioGroup.add(bgMaximizeRadio);
			bgRadioGroup.add(bgTileRadio);
			
			bgEqualRadio.addActionListener(new ActionListener() {
				@Override
				public void actionPerformed(ActionEvent e) {
					owner.getModel().setBgMode(FSModel.BG_MODE_EQUAL);
				}
			});
			
			bgTileRadio.addActionListener(new ActionListener() {
				@Override
				public void actionPerformed(ActionEvent e) {
					owner.getModel().setBgMode(FSModel.BG_MODE_TILE);
				}
			});
			
			bgMaximizeRadio.addActionListener(new ActionListener() {
				@Override
				public void actionPerformed(ActionEvent e) {
					owner.getModel().setBgMode(FSModel.BG_MODE_MAXIMIZE);
				}
			});
			
		} catch (Throwable e) {
			e.printStackTrace();
		}
	}
	
	private void imageSelected() {
		int id = selectorPanel.getSelectedImageId();
		if(id < 0) {
			return;
		}
		owner.getModel().setBackgroundImageId(id);
	}
	
	public void selectImage(int imageId) {
		selectorPanel.selectImage(imageId);
	}
	
	public void applyModel() {
		selectorPanel.selectImage(owner.getModel().getBackgroundImageId());
		switch(owner.getModel().getBgMode()) {
		case FSModel.BG_MODE_EQUAL :
			bgEqualRadio.setSelected(true);
			break;
		case FSModel.BG_MODE_MAXIMIZE :
			bgMaximizeRadio.setSelected(true);
			break;
		case FSModel.BG_MODE_TILE : 
			bgTileRadio.setSelected(true);
			break;
		}
	}
}
