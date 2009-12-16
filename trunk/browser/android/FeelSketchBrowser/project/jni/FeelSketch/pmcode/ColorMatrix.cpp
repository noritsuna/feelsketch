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

#include "ColorMatrix.h"

namespace pmcode {
using namespace common;

CvScalar ColorMatrix::get(CvPoint position) {
	Ref<Point> p(mPoints->get(position.x, position.y));
	CvScalar color = cvGet2D(mImage, p->getY(), p->getX());

	if (mIsPreparedAdjustment) {
		return mAdjustment.adjustColor(color);
	} else {
		return color;
	}
}

void ColorMatrix::set(CvPoint position, CvScalar color) {
	Ref<Point> p(mPoints->get(position.x, position.y));
	cvCircle(mImage, cvPoint(p->getX(), p->getY()), 1, color, -1);
}


void ColorMatrix::prepareLevelAdjustment(CvPoint blackPosition, CvPoint whitePosition, CvPoint greyPosition) {
	mAdjustment.prepareLut(get(blackPosition), get(whitePosition), get(greyPosition));
	mIsPreparedAdjustment = true;
}
}
