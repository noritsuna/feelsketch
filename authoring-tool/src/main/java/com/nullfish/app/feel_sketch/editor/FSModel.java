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

import java.io.OutputStream;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * The model class of Feel Sketch Panel class(FSPanel).
 * @author shunji
 *
 */
@SuppressWarnings("serial")
public class FSModel implements Serializable {
	/**
	 * BG image ID.
	 * 0 means no image.
	 */
	private int backgroundImageId = 0;

	/**
	 * BG image drawing mode.
	 */
	private int bgMode = BG_MODE_MAXIMIZE;

	/**
	 * Layer models.
	 */
	private List<LayerModel> layerModels = new ArrayList<LayerModel>();
	
	/**
	 * Listeners
	 */
	private transient List<FSModelListener> listeners = new ArrayList<FSModelListener>();
	
	/**
	 * Paper size
	 */
	private PaperSize paperSize = PaperSize.a4;
	
	/**
	 * Media data
	 */
	private String mediaData;
	
	/**
	 * The position of PM code.
	 */
	private PMCodePosition pmCodePosition = PMCodePosition.topLeft;
	
	public static final int BG_MODE_EQUAL = 0;
	public static final int BG_MODE_MAXIMIZE = 1;
	public static final int BG_MODE_TILE = 2;
	
	public FSModel() {
		addLayer(new LayerModel());
	}
	
	public void addListener(FSModelListener listener) {
		initListenersIfNecessary();
		listeners.add(listener);
	}
	
	public void removeListener(FSModelListener listener) {
		initListenersIfNecessary();
		listeners.remove(listener);
	}
	
	public void addLayer(LayerModel layerModel) {
		initListenersIfNecessary();
		layerModels.add(layerModel);
		for(FSModelListener listener : listeners) {
			listener.layerAdded(layerModel);
		}
	}
	
	public void removeLayer(LayerModel layerModel) {
		initListenersIfNecessary();
		layerModels.remove(layerModel);
		for(FSModelListener listener : listeners) {
			listener.layerRemoved(layerModel);
		}
	}
	
	public int getBackgroundImageId() {
		return backgroundImageId;
	}

	public void setBackgroundImageId(int backgroundImageId) {
		this.backgroundImageId = backgroundImageId;
		for(FSModelListener listener : listeners) {
			listener.backgroundImageChanged();
		}
	}
	
	public int getBgMode() {
		return bgMode;
	}

	public void setBgMode(int bgMode) {
		this.bgMode = bgMode;
		for(FSModelListener listener : listeners) {
			listener.backgroundImageChanged();
		}
	}

	public List<LayerModel> getLayersModel() {
		return layerModels;
	}

	private void initListenersIfNecessary() {
		if(listeners == null) {
			listeners = new ArrayList<FSModelListener>();
		}
	}
	
	public void writeStream(OutputStream os) {
		
	}
	
	public PaperSize getPaperSize() {
		return paperSize;
	}

	public void setPaperSize(PaperSize paperSize) {
		if(this.paperSize == paperSize) {
			return;
		}
		this.paperSize = paperSize;
		for(FSModelListener listener : listeners) {
			listener.sizeChanged();
		}
	}

	
	public PMCodePosition getPmCodePosition() {
		return pmCodePosition;
	}

	public void setPmCodePosition(PMCodePosition pmCodePosition) {
		this.pmCodePosition = pmCodePosition;
	}

	public String getMediaData() {
		return mediaData;
	}

	public void setMediaData(String mediaData) {
		this.mediaData = mediaData;
	}
	
	public int getIndexOfLayer(LayerModel layerModel) {
		return layerModels.indexOf(layerModel);
	}
	
	public void setIndexOfLayer(LayerModel layerModel, int index) {
		layerModels.remove(layerModel);
		layerModels.add(index, layerModel);
		for(FSModelListener listener : listeners) {
			listener.layerOrderChanged();
		}
	}
}
