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

import java.awt.Graphics;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.imageio.ImageIO;

import com.nullfish.app.feel_sketch.editor.NumberedColor;

public class BGImageThumbnailsCache {
	private Map<Integer, Image> idThumbnailMap = new HashMap<Integer, Image>();
	
	private Object syncObj = new Object();
	
	private Image loadingImage;
	
	private Image broakenImage;
	
	private Image noImage;
	
	private List<BGImageThumbnailsCacheListener> listeners = new ArrayList<BGImageThumbnailsCacheListener>();
	
	private static final BGImageThumbnailsCache instance = new BGImageThumbnailsCache();
	
	public static final String THUMBNAILS_FOLDER = "/resource/bg/thumbnail/";
	
	public static final String LOADING = "/resource/bg/thumbnail/loading.png";
	
	public static final String BROAKEN = "/resource/bg/thumbnail/broaken.png";
	
	public static final String NO_IMAGE = "/resource/bg/thumbnail/no_image.png";
	
	private BGImageThumbnailsCache() {
		try {
			loadingImage = ImageIO.read(getClass().getResourceAsStream(LOADING));
			broakenImage = ImageIO.read(getClass().getResourceAsStream(BROAKEN));
			noImage = ImageIO.read(getClass().getResourceAsStream(NO_IMAGE));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public static BGImageThumbnailsCache getInstance() {
		return instance;
	}
	
	public Image getImage(int id) {
		if(id == 0) {
			return noImage;
		}
		synchronized (syncObj) {
			Image rtn = idThumbnailMap.get(id);
			if(rtn != null) {
				return rtn;
			}
		}
		
		new ImageLoaderThread(id).start();
		return loadingImage;
	}
	
	private class ImageLoaderThread extends Thread {
		private int id;
		
		ImageLoaderThread(int id) {
			this.id = id;
		}
		
		public void run() {
			try {
				Image image;
				if(id < 32) {
					image = new BufferedImage(80, 60, BufferedImage.TYPE_INT_ARGB);
					Graphics g = image.getGraphics();
					g.setColor(NumberedColor.getColor(id));
					g.fillRect(0, 0, 80, 60);
				} else {
					image = ImageIO.read(getClass().getResourceAsStream(THUMBNAILS_FOLDER + id + ".png"));
				}
				synchronized (syncObj) {
					idThumbnailMap.put(id, image);
				}
			} catch (Exception e) {
				synchronized (syncObj) {
					idThumbnailMap.put(id, broakenImage);
				}
			}
			for(BGImageThumbnailsCacheListener listener : listeners) {
				listener.imageLoaded(id);
			}
		}
	}
	
	public void addListener(BGImageThumbnailsCacheListener listener) {
		listeners.add(listener);
	}
	
	public void removeListener(BGImageThumbnailsCacheListener listener) {
		listeners.remove(listener);
	}
}
