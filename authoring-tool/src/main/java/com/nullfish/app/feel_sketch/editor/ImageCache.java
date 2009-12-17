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

import java.awt.Graphics;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

import javax.imageio.ImageIO;

/**
 * A cache class of icon images and thumbnails.
 * @author shunji
 *
 */
public class ImageCache {
	private Map<Integer, Image> images = new HashMap<Integer, Image>();
	
	private Map<Integer, Image> thumbnails = new HashMap<Integer, Image>();
	
	private static ImageCache instance = new ImageCache();
	
	public static final int THUMBNAIL_WIDTH = 50;
	public static final int THUMBNAIL_HEIGHT = 50;
	
	private ImageCache() {
	}
	
	public static ImageCache getInstance() {
		return instance;
	}
	
	public Image getImage(int imageId) {
		Image rtn = images.get(imageId);
		if(rtn == null) {
			try {
				rtn = ImageIO.read(ImageCache.class.getResourceAsStream("/resource/icons/" + imageId + ".png"));
			} catch (IOException e) {
				throw new IllegalStateException();
			}
			images.put(imageId, rtn);
		}
		return rtn;
	}
	
	public Image getThumbnails(int imageId) {
		Image rtn = thumbnails.get(imageId);
		if(rtn == null) {
			try {
				InputStream resource = ImageCache.class.getResourceAsStream("/resource/icons/thumbnails/" + imageId + ".png");
				if(resource != null) {
					rtn = ImageIO.read(resource);
				} else {
					BufferedImage image = ImageIO.read(ImageCache.class.getResourceAsStream("/resource/icons/" + imageId + ".png"));
					int w = image.getWidth();
					int h = image.getHeight();
					double wRatio = (double)THUMBNAIL_WIDTH / (double)w;
					wRatio = wRatio < 1 ? wRatio : 1;
					double hRatio = (double)THUMBNAIL_HEIGHT / (double)h;
					hRatio = hRatio < 1 ? hRatio : 1;
					double ratio = wRatio < hRatio ? wRatio : hRatio;
					
					rtn = new BufferedImage((int)(w * ratio), (int)(h * ratio), BufferedImage.TYPE_INT_ARGB);
					Graphics g = rtn.getGraphics();
					g.drawImage(image, 0, 0, (int)(w * ratio), (int)(h * ratio), null);
					
					image.flush();
					g.dispose();
				}
				
			} catch (IOException e) {
				throw new IllegalStateException();
			}
			thumbnails.put(imageId, rtn);
		}
		return rtn;
	}
}
