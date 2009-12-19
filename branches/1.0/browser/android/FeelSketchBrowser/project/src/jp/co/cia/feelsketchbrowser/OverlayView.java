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
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Rect;
import android.view.View;

public class OverlayView extends View {
    Bitmap mImage;
    Rect mRect;
    int mWidth;
    int mHeight;

    public OverlayView(Context c)
    {
        super(c);
        mRect = new Rect(0, 0, 0, 0);
    }
    
    public void setRect(Rect r) {
    	mRect = new Rect(r);
    }
    
    public void setImage(Bitmap img) {
    	mImage = img;
    }
    
    protected void onDraw(Canvas canvas)
    {
        super.onDraw(canvas);
        
        canvas.drawColor(Color.TRANSPARENT);
        
        if (mRect.right != 0) {
        	canvas.drawBitmap(mImage, null, mRect, null);
        }
    }
}
