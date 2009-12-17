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

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.JComboBox;
import javax.swing.JPanel;
import javax.swing.JRadioButton;

import com.nullfish.app.feel_sketch.editor.FSModel;
import com.nullfish.app.feel_sketch.editor.PMCodePosition;
import com.nullfish.app.feel_sketch.editor.PaperSize;
import com.nullfish.app.feel_sketch.ui.FSResource;

/**
 * A palette panel class of preferences of a FSPanel.
 * @author shunji
 *
 */
@SuppressWarnings("serial")
public class PrefPalette extends JPanel {
	private Palette palette;
	
	/**
	 * button group of paper size radio buttons.
	 */
	private ButtonGroup group = new ButtonGroup();
	
	private JRadioButton lRadio = new JRadioButton(FSResource.getString("size_l"));
	private JRadioButton postCardRadio = new JRadioButton(FSResource.getString("size_post_card"));
	private JRadioButton a4Radio = new JRadioButton(FSResource.getString("size_a4"));
	private JRadioButton b5Radio = new JRadioButton(FSResource.getString("size_b5"));
	
	/**
	 * a combo box of PM-code position.
	 */
	JComboBox pmPosCombo = new JComboBox();
	
	private JRadioButton[] radios = {
			lRadio, postCardRadio, a4Radio, b5Radio
	};
	
	public PrefPalette(Palette palette) {
		this.palette = palette;
		
		initGui();
	}
	
	private void initGui() {
		setLayout(new GridBagLayout());
		JPanel buttonsPanel = new JPanel();
		BoxLayout buttonslayout = new BoxLayout(buttonsPanel, BoxLayout.Y_AXIS);
		buttonsPanel.setLayout(buttonslayout);
		buttonsPanel.setBorder(BorderFactory.createTitledBorder(FSResource.getString("image_size")));
		
		for(JRadioButton radio : radios) {
			group.add(radio);
			buttonsPanel.add(radio);
			radio.addActionListener(new ActionListener() {
				
				@Override
				public void actionPerformed(ActionEvent e) {
					selectionChanged();
				}
			});
		}
		
		this.add(buttonsPanel, new GridBagConstraints(0, 0, 1, 1, 1.0, 0.0, GridBagConstraints.NORTHWEST, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));
		
		pmPosCombo.addItem(PMCodePosition.topLeft); 
		pmPosCombo.addItem(PMCodePosition.centerLeft); 
		pmPosCombo.addItem(PMCodePosition.bottomLeft); 
		pmPosCombo.addItem(PMCodePosition.topRight); 
		pmPosCombo.addItem(PMCodePosition.centerRight); 
		pmPosCombo.addItem(PMCodePosition.bottomRight); 
		pmPosCombo.addItem(PMCodePosition.topCenter); 
		pmPosCombo.addItem(PMCodePosition.centerCenter); 
		pmPosCombo.addItem(PMCodePosition.bottomCenter);
		
		pmPosCombo.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				palette.getOwner().getModel().setPmCodePosition((PMCodePosition) pmPosCombo.getSelectedItem());
			}
		});
		
		JPanel pmPosPanel = new JPanel();
		pmPosPanel.setBorder(BorderFactory.createTitledBorder(FSResource.getString("pm_position")));
		pmPosPanel.add(pmPosCombo);
		this.add(pmPosPanel, new GridBagConstraints(0, 1, 1, 1, 1.0, 0.0, GridBagConstraints.NORTHWEST, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));
		this.add(new JPanel(), new GridBagConstraints(0, 2, 1, 1, 1.0, 1.0, GridBagConstraints.NORTHWEST, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));
	}

	protected void selectionChanged() {
		if(lRadio.isSelected()) {
			palette.getOwner().getModel().setPaperSize(PaperSize.l);
		} else if(postCardRadio.isSelected()) {
			palette.getOwner().getModel().setPaperSize(PaperSize.postCard);
		} else if(a4Radio.isSelected()) {
			palette.getOwner().getModel().setPaperSize(PaperSize.a4);
		} else if(b5Radio.isSelected()) {
			palette.getOwner().getModel().setPaperSize(PaperSize.b5);
		}
	}
	
	public void applyModel(FSModel model) {
		PaperSize paperSize = model.getPaperSize();
		if(paperSize == PaperSize.l) {
			lRadio.setSelected(true);
		} else if(paperSize == PaperSize.postCard) {
			postCardRadio.setSelected(true);
		} else if(paperSize == PaperSize.a4) {
			a4Radio.setSelected(true);
		} else if(paperSize == PaperSize.a4) {
			a4Radio.setSelected(true);
		}
		
		pmPosCombo.setSelectedItem(model.getPmCodePosition());
	}
}
