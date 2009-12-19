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

#ifndef __COLOR_MATRIX_H__
#define __COLOR_MATRIX_H__

#include <cv.h>
#include "util.h"
#include "PointMatrix.h"
#include "LevelAdjustment.h"

namespace pmcode {
using namespace common;

class ColorMatrix {
private:
	IplImage* mImage;
	Ref<PointMatrix> mPoints;
	LevelAdjustment mAdjustment;
	bool mIsPreparedAdjustment;
public:
    ColorMatrix(IplImage* image, Ref<PointMatrix> points) :
    	mImage(image), mPoints(points) { mIsPreparedAdjustment = false; }
    ~ColorMatrix() { }
    CvScalar get(CvPoint position);
    void set(CvPoint position, CvScalar color);
    void prepareLevelAdjustment(CvPoint blackPosition, CvPoint whitePosition, CvPoint greyPosition);
    int getDimension() { return mPoints->getDimension(); }
};
}

#endif // __POINT_MATRIX_H__
