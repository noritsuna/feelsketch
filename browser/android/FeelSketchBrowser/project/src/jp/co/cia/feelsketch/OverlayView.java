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

import java.util.Timer;
import java.util.TimerTask;

import android.content.Context;
//import android.content.res.Resources;
import android.graphics.Bitmap;
//import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Rect;
import android.view.View;

public class OverlayView extends View {
    Bitmap images[];
    Rect rect;
    Timer timer;
    int width;
    int height;
    int imageNum;
    final static int ANIMATION_PERIOD = 300;

    public OverlayView(Context c)
    {
        super(c);
        rect = new Rect(0, 0, 0, 0);
    }
    
    public void setRect(Rect r) {
    	rect = new Rect(r);
    }
    
    public void setImages(Bitmap[] imgs) {
    	images = imgs;
    }
    
    public void startAnimation() {
    	imageNum = 0;
    	timer = new Timer(true);
        final android.os.Handler handler = new android.os.Handler();
        timer.schedule(
        	new TimerTask() {
        		@Override
        		public void run() {
        			handler.post( new Runnable(){
        				public void run(){
        					invalidate();
        					imageNum++;
        					if (imageNum == 4) {
        						imageNum = 0;
        					}
        				}
        			});
        		}
        	}, 0, ANIMATION_PERIOD);
    }
    
    protected void onDraw(Canvas canvas)
    {
        super.onDraw(canvas);
        
        canvas.drawColor(Color.TRANSPARENT);
        
        if (rect.right != 0) {
        	canvas.drawBitmap(images[imageNum], null, rect, null);
        }
    }
}
