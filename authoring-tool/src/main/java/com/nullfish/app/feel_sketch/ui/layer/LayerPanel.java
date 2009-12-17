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

package com.nullfish.app.feel_sketch.ui.layer;

import java.awt.CardLayout;
import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.AbstractAction;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import com.nullfish.app.feel_sketch.editor.ElementModel;
import com.nullfish.app.feel_sketch.editor.LayerModel;
import com.nullfish.app.feel_sketch.editor.LayerModelListener;
import com.nullfish.app.feel_sketch.ui.FSResource;

/**
 * A editor panel of each layer.
 * @author shunji
 *
 */
@SuppressWarnings("serial")
public class LayerPanel extends JPanel {
	private LayerSelectorPanel layerSelectorPanel;

	/**
	 * A model class of layer
	 */
	private LayerModel model;
	
	/**
	 * a layout manager which switches a name label and a text field.
	 */
	private CardLayout namePanelLayout = new CardLayout();
	
	/**
	 * a panel for a name label and a text field.
	 */
	private JPanel namePanel = new JPanel(namePanelLayout);
	
	/**
	 * a label for layer name.
	 */
	private JLabel nameLabel = new JLabel();
	
	/**
	 * a text field for layer name.
	 */
	private JTextField nameText = new JTextField();
	
	/**
	 * a check box of layer visibility.
	 */
	private JCheckBox visibleCheck = new JCheckBox();
	
	/**
	 * a check box of containing the layer in a PM-code or not.
	 */
	private JCheckBox pmCodeCheck = new JCheckBox();
	
	/**
	 * a button to remove the layer from the model.
	 */
	private JButton removeButton = new JButton(new RemoveAction());
	
	private static ImageIcon visibleIcon = new ImageIcon(LayerPanel.class.getResource("/resource/buttons/visible.png"));
	private static ImageIcon invisibleIcon = new ImageIcon(LayerPanel.class.getResource("/resource/buttons/invisible.png"));
	
	private static ImageIcon exportableIcon = new ImageIcon(LayerPanel.class.getResource("/resource/buttons/exportable.png"));
	private static ImageIcon notExportableIcon = new ImageIcon(LayerPanel.class.getResource("/resource/buttons/not_exportable.png"));

	private static final String LABEL = "label";
	private static final String TEXT = "text";

	
	public LayerPanel(LayerSelectorPanel layerSelectorPanel, LayerModel model) {
		this.layerSelectorPanel = layerSelectorPanel;
		this.model = model;
		
		initGui();
		nameLabel.setText(model.getName());
		pmCodeCheck.setSelected(model.isToBePMCode());
	}
	
	private void initGui() {
		visibleCheck.setSelectedIcon(visibleIcon);
		visibleCheck.setIcon(invisibleIcon);
		visibleCheck.setToolTipText(FSResource.getString("layer_visibility"));
		
		pmCodeCheck.setSelectedIcon(exportableIcon);
		pmCodeCheck.setIcon(notExportableIcon);
		pmCodeCheck.setToolTipText(FSResource.getString("exportable"));
		
		namePanel.setOpaque(false);
		visibleCheck.setSelected(true);
		
		removeButton.setHideActionText(true);
		
		nameText.setVisible(false);

		namePanel.add(nameLabel, LABEL);
		namePanel.add(nameText, TEXT);
		namePanelLayout.show(namePanel, LABEL);
		
		setLayout(new GridBagLayout());
		add(visibleCheck, new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0, GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(2, 2, 2, 2), 0, 0));
		add(pmCodeCheck, new GridBagConstraints(1, 0, 1, 1, 0.0, 0.0, GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(2, 2, 2, 2), 0, 0));
		add(namePanel, new GridBagConstraints(2, 0, 1, 1, 1.0, 0.0, GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(2, 2, 2, 2), 0, 0));
		add(removeButton, new GridBagConstraints(3, 0, 1, 1, 0.0, 0.0, GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(2, 2, 2, 2), 0, 0));

		model.addListener(new LayerModelListener() {
			
			@Override
			public void toBePMCodedChanged() {}
			
			@Override
			public void nameChanged() {
				nameLabel.setText(model.getName());
			}
			
			@Override
			public void modelRemoved(ElementModel model) {}
			
			@Override
			public void modelAdded(ElementModel model) {}
			
			@Override
			public void effectChanged() {}
		});
		
		addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				layerSelectorPanel.setActiveLayer(getModel());
				if(e.getClickCount() == 2) {
					showNameText();
				}
			}
		});
		
		nameLabel.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				layerSelectorPanel.setActiveLayer(getModel());
				if(e.getClickCount() == 2) {
					showNameText();
				}
			}
		});
		
		visibleCheck.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				layerSelectorPanel.getOwner().repaint();
			}
		});
		
		nameText.addFocusListener(new FocusListener() {
			
			@Override
			public void focusLost(FocusEvent e) {
				namePanelLayout.show(namePanel, LABEL);
			}
			
			@Override
			public void focusGained(FocusEvent e) {
			}
		});
		
		nameText.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				model.setName(nameText.getText());
				namePanelLayout.show(namePanel, LABEL);
			}
		});
		
		pmCodeCheck.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				model.setToBePMCode(pmCodeCheck.isSelected());
			}
		});
	}
	
	private void showNameText() {
		nameText.setText(model.getName());
		namePanelLayout.show(namePanel, TEXT);
	}
	
	public void setActiveLayer(boolean active) {
		setBackground(active ? Color.BLUE : Color.WHITE);
	}
	
	public LayerModel getModel() {
		return model;
	}

	public boolean isLayerVisible() {
		return visibleCheck.isSelected();
	}
	
	private class RemoveAction extends AbstractAction {
		public RemoveAction() {
			super(FSResource.getString("remove_layer"), new ImageIcon(LayerPanel.class.getResource("/resource/buttons/delete.png")));
		}
		
		@Override
		public void actionPerformed(ActionEvent e) {
			layerSelectorPanel.getOwner().getModel().removeLayer(model);
		}
		
	}
}
