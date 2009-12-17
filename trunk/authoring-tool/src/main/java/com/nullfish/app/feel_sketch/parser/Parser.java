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

package com.nullfish.app.feel_sketch.parser;

import java.io.IOException;
import java.io.OutputStream;
import java.util.UUID;

import com.nullfish.app.feel_sketch.editor.ElementModel;
import com.nullfish.app.feel_sketch.editor.FSModel;
import com.nullfish.app.feel_sketch.editor.LayerModel;
import com.nullfish.app.feel_sketch.util.ByteConverter;

/**
 * A parser class of FSModel.
 * @author shunji
 *
 */
public class Parser {
	/**
	 * Feel Sketch version
	 */
	public static final byte VERSION = 1;
	
	/**
	 * The end command.
	 */
	public static final byte END = (byte)255;
	
	/**
	 * Outputs Feel Sketch model data to the stream.
	 * @param model
	 * @param os
	 * @throws IOException
	 */
	public void write(FSModel model, OutputStream os) throws IOException {
		
		// Feel Sketch version
		os.write(VERSION);
		
		// paper height
		os.write(ByteConverter.convert(model.getPaperSize().getHeight(), 2));
		
		// PM-Code size
		os.write((byte)2);
		
		// PM-Code position
		os.write((byte)model.getPmCodePosition().getValue());
		
		// paper size
		os.write((byte)model.getPaperSize().getValue());
		
		// background
		os.write((byte)model.getBackgroundImageId());
		
		// reserved.
		os.write((byte)0);
		
		// UUID
		UUID uuid = UUID.randomUUID();
		os.write(ByteConverter.convert(uuid.getMostSignificantBits(), 8));
		os.write(ByteConverter.convert(uuid.getLeastSignificantBits(), 8));
		
		// media data
		os.write(((model.getMediaData() != null ? model.getMediaData() : "") + "\r\n").getBytes());
		
		int layerOrder = 0;
		for(LayerModel layerModel : model.getLayersModel()) {
			if(!layerModel.isToBePMCode()) {
				continue;
			}
			
			// layer
			os.write((byte)0);
			os.write((byte)(layerOrder++ ^ (1 << 7)));
			os.write(ByteConverter.convert(layerModel.getEffect(), 2));
			
			for(ElementModel elementModel : layerModel.getModels()) {
				// each element
				os.write(elementModel.toBytes(model));
			}
		}
	}
}
