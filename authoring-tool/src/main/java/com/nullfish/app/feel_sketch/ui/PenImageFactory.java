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

package com.nullfish.app.feel_sketch.ui;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.util.HashMap;
import java.util.Map;

import com.nullfish.app.feel_sketch.editor.Pen;

public class PenImageFactory {
	private int imageSize = 30;
	
	private Map<Pen, BufferedImage> imageMap = new HashMap<Pen, BufferedImage>();
	
	public BufferedImage getImage(Pen pen) {
		BufferedImage image = imageMap.get(pen.getId());
		if(image == null) {
			image = new BufferedImage(imageSize, imageSize, BufferedImage.TYPE_INT_ARGB);
			Graphics2D g = image.createGraphics();
			g.setColor(Color.BLACK);
			g.setComposite(pen.getComposite());
			g.setStroke(new BasicStroke(pen.getRealWidth()));
			g.drawLine(1, imageSize / 2, imageSize - 1, imageSize / 2);
			g.dispose();
			imageMap.put(pen, image);
		}
		return image;
	}
}
