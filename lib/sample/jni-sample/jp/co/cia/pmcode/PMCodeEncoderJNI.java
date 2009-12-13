package jp.co.cia.pmcode;

public class PMCodeEncoderJNI {
	private byte[] image;
	private int width;
	private int height;
	
	public byte[] getImage() {
		return image;
	}

	public void setImage(byte[] image) {
		this.image = image;
	}

	public int getWidth() {
		return width;
	}

	public void setWidth(int width) {
		this.width = width;
	}

	public int getHeight() {
		return height;
	}

	public void setHeight(int height) {
		this.height = height;
	}

	static {
	    System.loadLibrary("PMCodeEncoderJNI");
	}
	public native int encode(byte[] data, PMCodeEncoderJNI result);
}
