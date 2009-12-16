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

package jp.co.cia.feelsketch;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.Bitmap.Config;
import android.hardware.Camera;
import android.os.Handler;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import static jp.co.cia.feelsketch.FeelSketchBrowser.TAG;

// プレビューの表示とデータの解析を行う
class Preview extends SurfaceView implements SurfaceHolder.Callback, Runnable {

    private FeelSketchBrowser mFSBrowser;
    private Camera mCamera;
    private FinderView mFinder;
    private SurfaceHolder mHolder;
    private Rect mFramingRect;
    private Point mResolution;
    private boolean mIsPreviewing = false;
    private ColorSettings mColorSettings;
    private OverlayView mOverlayView;
    private boolean mIsOverlay = false;
    private int mAutofocusCount = 0;
    
    private final Handler handler = new Handler();
    private DecodePMCodeThread mDecodeThread;
    private PMResult mPMResult;
    private FSResult mFSResult;

    public Preview(Context context, AttributeSet attrs) {
        super(context, attrs);
        Log.i(TAG, "new Preview");

        mFSBrowser = (FeelSketchBrowser) context;

        mHolder = getHolder();
        mHolder.addCallback(this);
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
        
        mColorSettings = new ColorSettings(context.getResources());
        
        mDecodeThread = new DecodePMCodeThread(handler, this);
    }
    
    public void setOverlayView(OverlayView overlayView) {
    	mOverlayView = overlayView;
    }
    
    public void setPMResult(PMResult pmResult) {
    	mPMResult = pmResult;
    }

    public void surfaceCreated(SurfaceHolder holder) {
        Log.i(TAG, "surfaceCreated");
        mCamera = Camera.open();
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.i(TAG, "surfaceDestroyed");
        if (mCamera != null && mIsPreviewing) {
            mIsPreviewing = false;
            closeCamera();
        }
    }

    public void surfaceChanged(SurfaceHolder holder, int format,
                               int w, int h) {
        Log.i(TAG, "surfaceChanged / w:" + w + ", h:" + h);

        if (mCamera != null) {
            if (mIsPreviewing) {
                mCamera.stopPreview();
            }

            // camera parameter setting
            Camera.Parameters parameters = mCamera.getParameters();
            parameters.setPictureSize(1024, 768);
            mCamera.setParameters(parameters);
            try {
				mCamera.setPreviewDisplay(holder);
			} catch (Exception e) {
				e.printStackTrace();
			}

            mCamera.startPreview();
            mIsPreviewing = true;
        }

        refreshScreenResolution();
        refreshFrameRect(false);
    }

    void setFinder(FinderView finder) {
        mFinder = finder;
    }

    public void closeCamera() {
        if (mCamera != null) {
        	mCamera.autoFocus(null);
            mCamera.stopPreview();
            mCamera.release();
            mCamera = null;
        }
    }

    // set finder region
    private void refreshFrameRect(boolean isOverlay) {
        if (mFinder != null) {
        	if (isOverlay == false) {
        		mFinder.setFramingRect(getFrameRect());
        	} else {
                Camera.Size size = mCamera.getParameters().getPreviewSize();
        		mFinder.setFramingRect(new Rect(0, 0, size.width, size.height));
        	}
        }
    }

    // create finder region
    private Rect getFrameRect() {
        Log.i(TAG, "getFramingRect");
        if (mFramingRect == null) {
            int size = 
                ((mResolution.x < mResolution.y) ? 
                    mResolution.x : mResolution.y);
            int leftOffset = (mResolution.x - size) / 2;
            int topOffset = (mResolution.y - size) / 2;
            mFramingRect = 
                new Rect(leftOffset, topOffset, leftOffset + size,
                         topOffset + size);
        }
        return mFramingRect;
    }

    private void refreshScreenResolution() {
        int w = getWidth();
        int h = getHeight();
        Log.i(TAG, "refreshScreenResolution(w:" + w + ", h:" + h);
        mResolution = new Point(w, h);
    }

    // start auto focus
    void requestAutoFocus() {
        Log.i(TAG, "requestAutoFocus");
        if (mCamera != null) {
        	if (!mIsPreviewing) {
        		mCamera.startPreview();
        		mIsPreviewing = true;
        	}
            mCamera.autoFocus(autoFocusCallback);
        }
    }

    void requestPreview(boolean isCallback) {
        Log.i(TAG, "requestPreview");
        if (mCamera != null) {
        	if (!mIsPreviewing) {
        		mCamera.startPreview();
        		mIsPreviewing = true;
        	}
        	if (isCallback) {
        		mCamera.setPreviewCallback(previewCallback);
        	}
        }
    }
    
    static private void decodeYUV420SP(int[] rgb, byte[] yuv420sp, int width, int height) {
    	final int frameSize = width * height;
    	
    	for (int j = 0, yp = 0; j < height; j++) {
    		int uvp = frameSize + (j >> 1) * width, u = 0, v = 0;
    		for (int i = 0; i < width; i++, yp++) {
    			int y = (0xff & ((int) yuv420sp[yp])) - 16;
    			if (y < 0) y = 0;
    			if ((i & 1) == 0) {
    				v = (0xff & yuv420sp[uvp++]) - 128;
    				u = (0xff & yuv420sp[uvp++]) - 128;
    			}
    			
    			int y1192 = 1192 * y;
    			int r = (y1192 + 1634 * v);
    			int g = (y1192 - 833 * v - 400 * u);
    			int b = (y1192 + 2066 * u);
    			
    			if (r < 0) r = 0; else if (r > 262143) r = 262143;
    			if (g < 0) g = 0; else if (g > 262143) g = 262143;
    			if (b < 0) b = 0; else if (b > 262143) b = 262143;
    			
    			rgb[yp] = 0xff000000 | ((r << 6) & 0xff0000) | ((g >> 2) & 0xff00) | ((b >> 10) & 0xff);
    		}
    	}
    }


    private final Camera.PreviewCallback previewCallback = 
        new Camera.PreviewCallback() {
            public void onPreviewFrame(byte[] data, Camera camera) {
            	mIsOverlay = true;
            	mFinder.setVisibility(View.INVISIBLE);
          	  	mCamera.setPreviewCallback(null);
                Camera.Size size = mCamera.getParameters().getPreviewSize();
          	  	int rgb[] = new int[size.width * size.height];
          	  	decodeYUV420SP(rgb, data, size.width, size.height);
          	  
                int result[] = getRect(rgb, size.width, size.height, mFSResult.pmcodeSize, mFSResult.pmcodePos, mFSResult.paperSize);
                rgb = null;
                Log.i(TAG, "rect : (" + result[0] + ", " + result[1] + ", " + result[2] + ", " + result[3] + ")");
                if (result[0] != 0 || mAutofocusCount == 5) {
                	mAutofocusCount = 0;
                	mOverlayView.setRect(new Rect(result[0], result[1], result[2], result[3]));
                } else {
                	mAutofocusCount++;
                }
             	requestAutoFocus();
          }
    };

    public void takePicture() {
    	mCamera.takePicture(null,null,new Camera.PictureCallback() {
            public void onPictureTaken(byte[] data,Camera camera) {
                try {
                	mFSBrowser.showProgressDialog();
                	camera.setPreviewCallback(null);
                	if (mIsPreviewing) {
                		camera.stopPreview();
                		mIsPreviewing = false;
                	}
                	Camera.Size size = camera.getParameters().getPictureSize();
                	
                	mDecodeThread.setDecodeParams(data, size, mColorSettings);
                    mDecodeThread.start();
                	
                } catch (Exception e) {
                	e.printStackTrace();
                }
            }
        }); 
    }
    
    public void run()
    {
    	try {
    		mDecodeThread = null;
    		mDecodeThread = new DecodePMCodeThread(handler, this);
    		
    		if ("fs".equals(mPMResult.extension)) {
        		int width = getWidth();
        		int height = getHeight();
                Log.i("PMCode", "width : " + width + ", height : " + height);
        		mFSResult = new FSResult();
        		getFSResult(mPMResult.data, mPMResult.data.length, mFSResult, width, height);
        		Bitmap[] bitmaps = new Bitmap[4];
        		for (int i = 0; i < 4; i++) {
        			int[] image = mFSResult.imageData[i];
        			bitmaps[i] = Bitmap.createBitmap(image, width, height, Config.ARGB_8888);
        		}
        		mOverlayView.setImages(bitmaps);
        		mOverlayView.startAnimation();

            	mFSBrowser.hideShutterButton();
        		requestPreview(true);
    		} else {
    			mFSBrowser.showIncorrectDialog(0);
    		}
            mFSBrowser.stopProgressDialog();
    	} catch (Exception e) {
        	e.printStackTrace();
        }
    }

    
    private final Camera.AutoFocusCallback autoFocusCallback = 
        new Camera.AutoFocusCallback() {
            public void onAutoFocus(boolean success, Camera camera) {
                Log.i(TAG, "onAutoFocus");
                camera.autoFocus(null);
                if (mIsOverlay) {
                	// get preview image and get AR marker
                	requestPreview(true);
                } else if (success){
                	takePicture();
                } else {
                	requestAutoFocus();
                }
            }
        };
        
        public native int[] getRect(int[] pixels, int width, int height, int pmCodeSize, int pmCodePos, int paperSize);
        public native void getFSResult(byte[] data, int dataSize, FSResult fsResult, int imageWidth, int imageHeight);
        
        static {
            System.loadLibrary("FeelSketch");
        }
}
