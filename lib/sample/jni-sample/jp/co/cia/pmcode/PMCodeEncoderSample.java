/**
***                  "Feel Sketch" PMCode Encoder & Decoder.
***    Copyright (C) 2009, Content Idea of ASIA Co.,Ltd. (oss.pmcode@ci-a.com)
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

package jp.co.cia.pmcode;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class PMCodeEncoderSample {

	public static void main(String[] args) {
		String src = "Hello PMCode!";
		byte[] data = src.getBytes();
		
		PMCodeEncoderJNI encoder = new PMCodeEncoderJNI();
		int ret = encoder.encode(data, encoder);
		if (ret != 0) {
			System.out.println("Encode Error! return code is " + ret);
			return;
		}
		System.out.println("widht = " + encoder.getWidth() + ", height = " + encoder.getHeight() + "\n");
		
		byte[] image = encoder.getImage();
		int width = encoder.getWidth();
		int height = encoder.getHeight();
		int[] rgbArray = new int[width*height];
		int index = 0;
		for (int rgb_index = 0; rgb_index < rgbArray.length; rgb_index++ ) {
          // red
        	rgbArray[rgb_index] |= (image[index] << 16 & 0xFF0000);
          // green
        	rgbArray[rgb_index] |= (image[index + 1] << 8 & 0x00FF00);
          // blue
        	rgbArray[rgb_index] |= (image[index + 2] & 0x0000FF);
            index = index + 3;
	    }
		
		// create image file.
		BufferedImage bi = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
		bi.setRGB(0, 0, width, height, rgbArray, 0, width);
		try {
			ImageIO.write(bi, "bmp", new File("pmcode.bmp"));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
