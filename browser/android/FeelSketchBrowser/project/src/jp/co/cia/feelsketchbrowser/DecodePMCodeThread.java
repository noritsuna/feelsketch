/**
***                          "Feel Sketch" browser.
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

package jp.co.cia.feelsketchbrowser;

import static jp.co.cia.feelsketchbrowser.FeelSketchBrowser.TAG;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Camera;
import android.os.Handler;
import android.util.Log;

public class DecodePMCodeThread extends Thread
{
    private Handler mHandler;

    private final Runnable mListener;
    
    private byte[] mImageData;
    private Camera.Size mImageSize;
    private ColorSettings mColorSettings;

    public DecodePMCodeThread(Handler handler, Runnable listener)
    {
        this.mHandler = handler;
        this.mListener = listener;
    }
    
    public void setDecodeParams(byte[] data, Camera.Size size, ColorSettings settings) {
    	mImageData = data;
    	mImageSize = size;
    	mColorSettings = settings;
    }

    @Override
    public void run()
    {
        int rectSize = 
            ((mImageSize.width < mImageSize.height) ? 
                mImageSize.width : mImageSize.height);
        Log.i(TAG, "takePicture: mWidth=" + mImageSize.width + ", mHeight=" + mImageSize.height + ", rectSize=" + rectSize + ", data.length=" + mImageData.length);
        int leftOffset = (mImageSize.width - rectSize) / 2;
        int topOffset = (mImageSize.height - rectSize) / 2;
  	  
        int argb[] = new int[mImageSize.width * mImageSize.height];
  	  	Bitmap bitmap = BitmapFactory.decodeByteArray(mImageData, 0, mImageData.length);
  	  	bitmap.getPixels(argb, 0, bitmap.getWidth(), 0, 0, bitmap.getWidth(), bitmap.getHeight());
  	  
  	  	PMResult pmResult = new PMResult();
  	  	int error = getPMResult(argb, mImageSize.width, mImageSize.height, leftOffset, topOffset, rectSize, pmResult, mColorSettings);
  	  	if (error == 0) {
  	  		pmResult.extension = "fs";
  	  	}
  	  	argb = null;
  	  	bitmap = null;
  	  
  	  	((Preview)mListener).setPMResult(pmResult);
  	  
        mHandler.post(mListener);
    }
    
    public native int getPMResult(int[] pixels, int width, int height, int left, int top, int size, PMResult pmResult, ColorSettings colorDefinition);
    
    static {
        System.loadLibrary("FeelSketch");
    }
}
