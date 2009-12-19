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

#ifndef __COLOR_DETECTOR_H__
#define __COLOR_DETECTOR_H__

#include <cv.h>
#include "PointMatrix.h"
#include "OutputColor.h"
#include "ColorMatrix.h"

namespace pmcode {
using namespace common;

class ColorDetector {
private:
	static const CvPoint COLOR_NUM_DECISION_POSITION;
	static const CvPoint BLACK_POSITION;
	static const CvPoint WHITE_POSITION;
	static const CvPoint GREY_POSITION;
	static const CvPoint BASE_COLOR_POSITIONS[16];

	int mColorNum;
	ColorMatrix mColorMatrix;
	OutputColor mOutputColor;
    int determineColorNum();
    void getBaseColors(CvScalar* resultColors);
    CvScalar selectColor(CvScalar color, CvScalar* baseColors);
	double distanceBetweenColors(CvScalar color1, CvScalar color2);
public:
    ColorDetector(IplImage* image, Ref<PointMatrix> points, OutputColor outputColor);
    ~ColorDetector();
    void detect(unsigned char *resultData);
    int getLayerNum();
};
}

#endif // __COLOR_DETECTOR_H__
