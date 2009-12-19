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

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.AbstractAction;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import com.nullfish.app.feel_sketch.editor.FSModel;
import com.nullfish.app.feel_sketch.editor.FSModelListener;
import com.nullfish.app.feel_sketch.editor.FSPanel;
import com.nullfish.app.feel_sketch.editor.LayerModel;
import com.nullfish.app.feel_sketch.editor.LayerSelector;
import com.nullfish.app.feel_sketch.ui.FSResource;

/**
 * An implementation of LayerSelector.
 * @author shunji
 *
 */
@SuppressWarnings("serial")
public class LayerSelectorPanel extends JPanel implements LayerSelector {
	private FSPanel owner;
	
	private FSModel model;
	
	/**
	 * a panel of current active layer. 
	 */
	private LayerPanel activePanel;
	
	private ModelListener listener = new ModelListener();
	
	private List<LayerPanel> layerPanels = new ArrayList<LayerPanel>();
	
	/**
	 * A map of models and panels of models. 
	 */
	private Map<LayerModel, LayerPanel> layerMap = new HashMap<LayerModel, LayerPanel>();
	
	/**
	 * A panel which contains all LayerPanels
	 */
	private JPanel layersPanel = new JPanel(new GridBagLayout());
	
	private JScrollPane scroll = new JScrollPane(layersPanel);

	private JButton addButton = new JButton(new AddAction());
	private JButton upButton = new JButton(new UpAction());
	private JButton downButton = new JButton(new DownAction());
	
	private JPanel filling = new JPanel();
	
	public LayerSelectorPanel(FSPanel owner) {
		this.owner = owner;
		
		setPreferredSize(new Dimension(200, 10));

		addButton.setHideActionText(true);
		upButton.setHideActionText(true);
		downButton.setHideActionText(true);
		
		setLayout(new BorderLayout(2, 2));

		JPanel buttonsPanel = new JPanel();
		buttonsPanel.setLayout(new BoxLayout(buttonsPanel, BoxLayout.X_AXIS));
		buttonsPanel.add(upButton);
		buttonsPanel.add(downButton);
		buttonsPanel.add(Box.createGlue());
		buttonsPanel.add(addButton);
		
		add(buttonsPanel, BorderLayout.NORTH);
		add(scroll, BorderLayout.CENTER);
		
	}
	
	@Override
	public void modelChanged() {
		if(model != null) {
			model.removeListener(listener);
		}
		layersPanel.removeAll();
		layerPanels.clear();
		layerMap.clear();
		
		model = owner.getModel();
		model.addListener(listener);
		
		for(LayerModel layerModel : model.getLayersModel()) {
			layerAdded(layerModel);
		}
		
		if(model.getLayersModel().size() > 0) {
			setActiveLayer(model.getLayersModel().get(0));
		}
	}
	
	private void orderLayerPanels() {
		layersPanel.removeAll();
		Collections.sort(layerPanels, new Comparator<LayerPanel>() {

			@Override
			public int compare(LayerPanel o1, LayerPanel o2) {
				return model.getIndexOfLayer(o2.getModel()) - model.getIndexOfLayer(o1.getModel());
			}
		});
		
		int y = 0;
		for(LayerPanel layerPanel : layerPanels) {
			layersPanel.add(layerPanel, new GridBagConstraints(0, y++, 1, 1, 1.0, 0.0, GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 0, 0));
		}
		
		layersPanel.add(filling, new GridBagConstraints(0, y++, 1, 1, 1.0, 1.0, GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 0, 0));
		layersPanel.revalidate();
		scroll.repaint();
	}
	
	private void layerAdded(LayerModel layerModel) {
		LayerPanel newPanel = new LayerPanel(this, layerModel);
		layerMap.put(layerModel, newPanel);
		layerPanels.add(newPanel);
		setActiveLayer(layerModel);
		orderLayerPanels();
	}
	
	private class AddAction extends AbstractAction {
		public AddAction() {
			super(FSResource.getString("add_layer"), new ImageIcon(LayerPanel.class.getResource("/resource/buttons/add.png")));
		}
		
		@Override
		public void actionPerformed(ActionEvent e) {
			String name = JOptionPane.showInputDialog(FSResource.getString("input_layer_name"));
			if(name == null) {
				return;
			}
			LayerModel newModel = new LayerModel();
			newModel.setName(name);
			model.addLayer(newModel);
		}
	}
	
	private class DownAction extends AbstractAction {

		public DownAction() {
			super(FSResource.getString("down"), new ImageIcon(LayerPanel.class.getResource("/resource/buttons/arrow_down.png")));
		}
		
		@Override
		public void actionPerformed(ActionEvent e) {
			if(activePanel == null) {
				return;
			}
			
			int index = model.getIndexOfLayer(activePanel.getModel());
			if(index <= 0) {
				return;
			}
			
			model.setIndexOfLayer(activePanel.getModel(), index - 1);
		}
	}
	
	private class UpAction extends AbstractAction {

		public UpAction() {
			super(FSResource.getString("up"), new ImageIcon(LayerPanel.class.getResource("/resource/buttons/arrow_up.png")));
		}
		
		@Override
		public void actionPerformed(ActionEvent e) {
			if(activePanel == null) {
				return;
			}
			
			int index = model.getIndexOfLayer(activePanel.getModel());
			if(index >= model.getLayersModel().size() - 1) {
				return;
			}
			
			model.setIndexOfLayer(activePanel.getModel(), index + 1);
		}
	}
	
	
	private class ModelListener implements FSModelListener {

		@Override
		public void layerAdded(LayerModel model) {
			LayerSelectorPanel.this.layerAdded(model);
		}

		@Override
		public void layerRemoved(LayerModel model) {
			LayerPanel panel = layerMap.get(model);
			layerPanels.remove(panel);
			layerMap.remove(model);
			orderLayerPanels();
		}

		@Override
		public void sizeChanged() {}
		
		@Override
		public void backgroundImageChanged() {}

		@Override
		public void layerOrderChanged() {
			orderLayerPanels();
		}

	}



	@Override
	public LayerModel getActiveLayer() {
		return activePanel.getModel();
	}

	public void setActiveLayer(LayerModel activeLayer) {
		if(activePanel != null) {
			activePanel.setActiveLayer(false);
		}
		
		activePanel = layerMap.get(activeLayer);
		activePanel.setActiveLayer(true);
	}
	
	@Override
	public boolean isLayerVisible(LayerModel layerModel) {
		return layerMap.get(layerModel).isLayerVisible();
	}
	
	public FSPanel getOwner() {
		return owner;
	}

}
