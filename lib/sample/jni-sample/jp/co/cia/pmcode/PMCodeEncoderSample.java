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
