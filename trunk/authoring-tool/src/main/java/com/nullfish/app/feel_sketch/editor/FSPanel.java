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

package com.nullfish.app.feel_sketch.editor;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.imageio.ImageIO;
import javax.swing.AbstractAction;
import javax.swing.JComponent;
import javax.swing.JPanel;
import javax.swing.KeyStroke;
import javax.swing.RepaintManager;

import com.nullfish.app.feel_sketch.editor.mode.BezierDrawLineMode;
import com.nullfish.app.feel_sketch.editor.mode.SelectingMode;

/**
 * The canvas class of Feel Sketch.
 * @author shunji
 *
 */
@SuppressWarnings("serial")
public class FSPanel extends JPanel {
	
	private FSModel model = new FSModel();
	
	/**
	 * a flag of outputting image.
	 */
	private boolean outputMode = false;
	
	/**
	 * a flag to draw layers which become PM-code only.
	 */
	private boolean outputNoPmCode = false;
	
	/**
	 * Current mode.
	 */
	private Mode mode = new BezierDrawLineMode(this, NumberedColor.BLACK, Pen.narrow);
	
	/**
	 * A model of elements selection.
	 */
	private SelectionModel selectionModel = new SelectionModel();

	private LayerSelector layerSelector;
	
	/**
	 * Layers.
	 */
	private List<Layer> layers = new ArrayList<Layer>();

	/**
	 * A map of models and layers.
	 */
	private Map<LayerModel, Layer> modelLayerMap = new HashMap<LayerModel, Layer>();
	
	/**
	 * A factory of elements.
	 */
	private ElementFactory elementFactory = new ElementFactory();
	
	/**
	 * A rectangle of area selection.
	 */
	private Rectangle selectionRectangle;
	
	/**
	 * Background image.
	 */
	private BufferedImage bgImage;
	
	/**
	 * Background color
	 */
	private Color bgColor;
	
	/**
	 * A painter class of elements selection.
	 */
	private SelectionPainter selectionPainter = new SelectionPainter(this);
	
	public static final Color SELECTION_COLOR = new Color(0, 0, 128);
	public static final float[] SELECTION_STROKE_DASH = {3.0f, 3.0f};
	public static final BasicStroke SELECTION_STROKE = new BasicStroke(1.0f, 
                                           BasicStroke.CAP_BUTT, 
                                           BasicStroke.JOIN_MITER, 
                                           10.0f, 
                                           SELECTION_STROKE_DASH, 
                                           0.0f);
	
	public static final Color SELECTION_RECT_COLOR = Color.YELLOW;
	public static final float[] SELECTION_RECT_STROKE_DASH = {3.0f, 3.0f};
	public static final BasicStroke SELECTION_RECT_STROKE = new BasicStroke(3.0f, 
                                           BasicStroke.CAP_BUTT, 
                                           BasicStroke.JOIN_MITER, 
                                           10.0f, 
                                           SELECTION_RECT_STROKE_DASH, 
                                           0.0f);
	
	public FSPanel() {
		setBackground(Color.WHITE);
		setOpaque(true);
		setModel(new FSModel());
		addMouseListener(new MouseListener() {
			
			@Override
			public void mouseReleased(MouseEvent e) {
				if(mode == null) {
					return;
				}
				
				mode.mouseReleased(e);
			}
			
			@Override
			public void mousePressed(MouseEvent e) {
				if(mode == null) {
					return;
				}
				
				mode.mousePressed(e);
			}
			
			@Override
			public void mouseExited(MouseEvent e) {
				if(mode == null) {
					return;
				}
				
				mode.mouseExited(e);
			}
			
			@Override
			public void mouseEntered(MouseEvent e) {
				if(mode == null) {
					return;
				}
				
				mode.mouseEntered(e);
			}
			
			@Override
			public void mouseClicked(MouseEvent e) {
				if(mode == null) {
					return;
				}
				
				mode.mouseClicked(e);
			}
		});
		
		addMouseMotionListener(new MouseMotionListener() {
			
			@Override
			public void mouseMoved(MouseEvent e) {
				if(mode == null) {
					return;
				}
				
				mode.mouseMoved(e);
			}
			
			@Override
			public void mouseDragged(MouseEvent e) {
				if(mode == null) {
					return;
				}
				
				mode.mouseDragged(e);
			}
		});
		
		selectionModel.addListener(new SelectionModelListener() {
			
			@Override
			public void selectionChanged(Element element) {
				Rectangle rect = SELECTION_STROKE.createStrokedShape(element.getShape()).getBounds();
				RepaintManager.currentManager(FSPanel.this).addDirtyRegion(FSPanel.this, rect.x, rect.y, rect.width, rect.height);
			}
		});
		
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0), "cancel");
		getActionMap().put("cancel", new AbstractAction() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				revertMode();
			}
		});
		
		revertMode();
	}
	
	public FSModel getModel() {
		return model;
	}
	
	public Layer getLayer(LayerModel layerModel) {
		return modelLayerMap.get(layerModel);
	}
	public void setModel(final FSModel model) {
		layers.clear();
		modelLayerMap.clear();
		model.addListener(new FSModelListener() {
			
			@Override
			public void layerRemoved(LayerModel layerModel) {
				FSPanel.this.layerRemoved(layerModel);
			}
			
			@Override
			public void layerAdded(LayerModel layerModel) {
				FSPanel.this.layerAdded(layerModel);
			}

			@Override
			public void backgroundImageChanged() {
				bgChanged();
			}

			@Override
			public void sizeChanged() {
				FSPanel.this.sizeChanged();
			}

			@Override
			public void layerOrderChanged() {
				Collections.sort(layers, new Comparator<Layer>() {

					@Override
					public int compare(Layer o1, Layer o2) {
						return model.getIndexOfLayer(o1.getModel()) - model.getIndexOfLayer(o2.getModel());
					}
				});
				repaint();
			}
		});
		
		this.model = model;
		selectionPainter.init();
		for(LayerModel m : model.getLayersModel()) {
			layerAdded(m);
		}
		
		sizeChanged();
		if(layerSelector != null) {
			layerSelector.modelChanged();
		}
	}
	
	private void sizeChanged() {
		setPreferredSize(new Dimension(640, 640 * model.getPaperSize().getHeight() / 256));
		bgChanged();
	}

	public List<Layer> getLayers() {
		return layers;
	}

	private void layerAdded(LayerModel layerModel) {
		Layer layer = new Layer(FSPanel.this, layerModel);
		modelLayerMap.put(layerModel, layer);
		layers.add(layer);
		repaint();
	}
	
	private void layerRemoved(LayerModel layerModel) {
		Layer layer = modelLayerMap.get(layerModel);
		layers.remove(layer);
		repaint();
		for(ElementModel model : layerModel.getModels()) {
			selectionModel.removeSelecetedElement(layer.getElementByModel(model));
		}
	}
	
	private void bgChanged() {
		bgColor = null;
		bgImage = null;
		if(bgImage != null) {
			bgImage.flush();
		}
		
		int bgId = model.getBackgroundImageId();
		if(bgId == 0) {
		} else if(bgId < 32) {
			bgColor = NumberedColor.getColor(bgId);
		} else {
			bgImage = new BufferedImage(getWidth(), getHeight(), BufferedImage.TYPE_INT_ARGB);
			try {
				BufferedImage image = ImageIO.read(FSPanel.class.getResource("/resource/bg/" + bgId + ".png"));
				Graphics g = bgImage.createGraphics();
				
				if(model.getBgMode() == FSModel.BG_MODE_EQUAL) {
					g.drawImage(image, (getWidth() - bgImage.getWidth()) / 2, (getHeight() - bgImage.getHeight()) / 2, this);
				} else if(model.getBgMode() == FSModel.BG_MODE_MAXIMIZE) {
					g.drawImage(image, 0, 0, getWidth(), getHeight(), this);
				} else if(model.getBgMode() == FSModel.BG_MODE_TILE) {
					int w = image.getWidth();
					int h = image.getHeight();
					
					for(int y = 0; y < getHeight(); y += h) {
						for(int x = 0; x < getWidth(); x += w) {
							g.drawImage(image, x, y, this);
						}
					}
				}
				image.flush();
				g.dispose();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		repaint();
	}

	@Override
	protected void paintComponent(Graphics g) {
		
		super.paintComponent(g);
		Graphics2D g2 = (Graphics2D) g;
		
		paintBg(g2);

		g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		
		for(Layer layer : layers) {
			if(outputMode) {
				if((!layer.getModel().isToBePMCode() || !outputNoPmCode )) {
					layer.paint(g2);
				}
			} else if(layerSelector == null || layerSelector.isLayerVisible(layer.getModel())) {
				layer.paint(g2);
			}
		}
		
		if(!outputMode) {
			selectionPainter.paint(g2);
			
			if(selectionRectangle != null) {
				g2.setColor(SELECTION_RECT_COLOR);
				g2.setStroke(SELECTION_RECT_STROKE);
				g2.draw(selectionRectangle);
			}
		}		
	}
	
	private void paintBg(Graphics2D g) {
		Rectangle clipRect = g.getClipBounds();
		if(bgColor != null) {
			g.setColor(bgColor);
			
			if(clipRect != null) {
				g.fill(clipRect);
			} else {
				g.fillRect(0, 0, getWidth(), getHeight());
			}
		}
		
		if(bgImage != null) {
			if(clipRect != null) {
				g.drawImage(bgImage, clipRect.x, clipRect.y, clipRect.x + clipRect.width, clipRect.y + clipRect.height, clipRect.x, clipRect.y, clipRect.x + clipRect.width, clipRect.y + clipRect.height, this);
			} else {
				g.drawImage(bgImage, 0, 0, this);
			}
		}
	}
	
	public void setMode(Mode mode) {
		if(this.mode != null) {
			this.mode.modeFinished();
		}
		this.mode = mode;
		if(mode != null) {
			mode.modeStarted();
			setCursor(mode.getMouseCursor());
		}
	}
	
	public void revertMode() {
		setMode(new SelectingMode(this));
	}
	
	public SelectionModel getSelectionModel() {
		return selectionModel;
	}
	
	public Element getElementAtPoint(Point p) {
		for(int i=0; i<layers.size(); i++) {
			Layer layer = layers.get(layers.size() - i - 1);
			Element e = layer.getElementAtPoint(p);
			if(e != null) {
				return e;
			}
		}
		return null;
	}

	public Element getElementAtPointOnActiveLayer(Point p) {
		if(layerSelector == null) {
			return null;
		}
		LayerModel layerModel = layerSelector.getActiveLayer();
		if(layerModel == null) {
			return null;
		}
		Layer layer = getLayer(layerModel);
		return layer.getElementAtPoint(p);
	}

	public void setSelectionRectangle(Rectangle selectionRectangle) {
		if(this.selectionRectangle != null) {
			Rectangle rect = SELECTION_RECT_STROKE.createStrokedShape(this.selectionRectangle).getBounds();
			RepaintManager.currentManager(this).addDirtyRegion(this, rect.x, rect.y, rect.width, rect.height);
		}
		this.selectionRectangle = selectionRectangle;
		if(this.selectionRectangle != null) {
			Rectangle rect = SELECTION_RECT_STROKE.createStrokedShape(this.selectionRectangle).getBounds();
			RepaintManager.currentManager(this).addDirtyRegion(this, rect.x, rect.y, rect.width, rect.height);
		}
	}
	
	public Element getElementByModel(ElementModel elementModel) {
		for(Layer layer : layers) {
			Element element = layer.getElementByModel(elementModel);
			if(element != null) {
				return element;
			}
		}
		return null;
	}
	
	public BufferedImage toImage() {
		return toImage(false);
	}

	public BufferedImage toImage(boolean noPmCode) {

		try {
			outputMode = true;
			outputNoPmCode = noPmCode;
			BufferedImage rtn = new BufferedImage(getWidth(), getHeight(), BufferedImage.TYPE_INT_ARGB);
			Graphics g = rtn.createGraphics();
			paintComponent(g);
			return rtn;
		} finally {
			outputMode = false;
			outputNoPmCode = false;
		}
	}

	public void setLayerSelector(LayerSelector layerSelector) {
		this.layerSelector = layerSelector;
		layerSelector.modelChanged();
		repaint();
	}

	public LayerSelector getLayerSelector() {
		return layerSelector;
	}
}
