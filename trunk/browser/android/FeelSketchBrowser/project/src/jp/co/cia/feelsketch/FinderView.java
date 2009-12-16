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
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import static jp.co.cia.feelsketch.FeelSketchBrowser.TAG;

public final class FinderView extends View {

    private final Paint mPaint;
    private final Rect mBox;
    private Rect mFrame;
    private final int mMaskColor;
    private final int mFrameColor;

    public FinderView(Context context, AttributeSet attrs) {
        super(context, attrs);
        Log.i(TAG, "new ViewfinderView");

        mPaint = new Paint();
        mBox = new Rect();
		
        Resources resources = getResources();
        mMaskColor = resources.getColor(R.color.finder_mask);
        resources.getColor(R.color.result_mask);
        mFrameColor = resources.getColor(R.color.finder_frame);
    }

    void setFramingRect(Rect frame) {
        mFrame = frame;
    }

    @Override
    public void onDraw(Canvas canvas) {

        if (mFrame == null) {
            return;
        }
		
        Log.e(TAG, "onDraw");
		
        Rect frame = mFrame;
        int width = canvas.getWidth();
        int height = canvas.getHeight();

        mPaint.setColor(mMaskColor);
        mBox.set(0, 0, width, frame.top);
        canvas.drawRect(mBox, mPaint);
        mBox.set(0, frame.top, frame.left, frame.bottom + 1);
        canvas.drawRect(mBox, mPaint);
        mBox.set(frame.right + 1, frame.top, width, frame.bottom + 1);
        canvas.drawRect(mBox, mPaint);
        mBox.set(0, frame.bottom + 1, width, height);
        canvas.drawRect(mBox, mPaint);

        mPaint.setColor(mFrameColor);
        mBox.set(frame.left, frame.top, frame.right + 1, frame.top + 2);
        canvas.drawRect(mBox, mPaint);
        mBox.set(frame.left, frame.top + 2, frame.left + 2, frame.bottom - 1);
        canvas.drawRect(mBox, mPaint);
        mBox.set(frame.right - 1, frame.top, frame.right + 1, frame.bottom - 1);
        canvas.drawRect(mBox, mPaint);
        mBox.set(frame.left, frame.bottom - 1, frame.right + 1,
                 frame.bottom + 1);
        canvas.drawRect(mBox, mPaint);
    }
}
