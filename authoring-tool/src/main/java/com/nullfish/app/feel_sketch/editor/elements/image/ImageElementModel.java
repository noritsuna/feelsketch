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

package com.nullfish.app.feel_sketch.editor.elements.image;

import java.awt.Point;
import java.io.ByteArrayOutputStream;
import java.io.IOException;

import com.nullfish.app.feel_sketch.editor.AbstractElementModel;
import com.nullfish.app.feel_sketch.editor.Locationable;
import com.nullfish.app.feel_sketch.editor.FSModel;
import com.nullfish.app.feel_sketch.util.ByteConverter;

/**
 * A model class of image 
 * @author shunji
 *
 */
@SuppressWarnings("serial")
public class ImageElementModel extends AbstractElementModel implements Locationable {
	private int imageId;
	
	private Point point;
	
	public static final String IMAGE_ID = "image_id";
	
	public ImageElementModel(int imageId) {
		int oldImageId = this.imageId;
		this.imageId = imageId;
		notifyChange(IMAGE_ID, oldImageId, imageId);
	}
	
	@Override
	public int getMemorySize() {
		return 0;
	}

	@Override
	public Point getLocation() {
		return point;
	}

	@Override
	public void setLocation(Point point) {
		Point oldValue = this.point;
		this.point = point;
		notifyChange(LOCATION, oldValue, point);
	}

	public int getImageId() {
		return imageId;
	}

	@Override
	public byte[] toBytes(FSModel model) throws IOException {
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		bos.write((byte)1);
		bos.write((byte)(point.getX() * 255 / 640));
		bos.write((byte)(point.getY() * 255 / (640 * model.getPaperSize().getHeight() / 254)));
		bos.write(ByteConverter.convert(imageId, 2));
		bos.write((byte)255);
		bos.write((byte)255);
		
		return bos.toByteArray();
	}
}
