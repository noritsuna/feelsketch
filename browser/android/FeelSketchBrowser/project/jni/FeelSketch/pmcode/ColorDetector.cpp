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

#include "ColorDetector.h"
#include <stdio.h>
#include "../util.h"

namespace pmcode {
const CvPoint ColorDetector::COLOR_NUM_DECISION_POSITION = cvPoint(2, 9);
const CvPoint ColorDetector::BLACK_POSITION = cvPoint(7, 7);
const CvPoint ColorDetector::WHITE_POSITION = cvPoint(3, 3);
const CvPoint ColorDetector::GREY_POSITION  = cvPoint(4, 9);
const CvPoint ColorDetector::BASE_COLOR_POSITIONS[16] = {
		cvPoint(4, 10),
		cvPoint(4, 11),
		cvPoint(4, 12),
		cvPoint(3, 9),
		cvPoint(3, 10),
		cvPoint(3, 11),
		cvPoint(3, 12),
		cvPoint(2, 9),
		cvPoint(2, 10),
		cvPoint(2, 11),
		cvPoint(2, 12),
		cvPoint(1, 9),
		cvPoint(1, 10),
		cvPoint(1, 11),
		cvPoint(1, 12),
		cvPoint(0, 9)
	};

using namespace common;

ColorDetector::ColorDetector(IplImage* image, Ref<PointMatrix> points, OutputColor outputColor) :
mOutputColor(outputColor), mColorMatrix(image, points) {
}

ColorDetector::~ColorDetector() {
}

void ColorDetector::detect(unsigned char *resultData) {
  	int x, y;
	char msg[128];
	CvScalar color;

	mColorMatrix.prepareLevelAdjustment(BLACK_POSITION, WHITE_POSITION, GREY_POSITION);

	mColorNum = determineColorNum();

	// get base colors from PM code header.
	CvScalar* baseColors = (CvScalar*)malloc(sizeof(CvScalar)*mColorNum);
	getBaseColors(baseColors);
	for (int i = 0; i < mColorNum; i++) {
		sprintf(msg, "%d: (%f, %f, %f)", i, baseColors[i].val[0], baseColors[i].val[1], baseColors[i].val[2]);
		LOG_FUNC_MESSAGE(msg);
	}

	int index = 0;
	int dimension = mColorMatrix.getDimension();
    for (y = 0; y < dimension; y++) {
        for (x = 0; x < dimension; x++) {
 			color = mColorMatrix.get(cvPoint(x, y));
			color = selectColor(color, baseColors);
			mColorMatrix.set(cvPoint(x, y), color);
			resultData[index]   = (unsigned char)((int)color.val[0]);
			resultData[index+1] = (unsigned char)((int)color.val[1]);
			resultData[index+2] = (unsigned char)((int)color.val[2]);
			index += 3;
        }
		resultData[index] = 0;
		index++;
    }
	free(baseColors);
}

int ColorDetector::getLayerNum() {
	int i = 0;
	int tmp = mColorNum;
	while (tmp / 2 > 0) {
		tmp /= 2;
		i++;
	}
	return i;
}

int ColorDetector::determineColorNum() {
	CvScalar color;
	color = mColorMatrix.get(COLOR_NUM_DECISION_POSITION);
	double blackDistance = distanceBetweenColors(color, cvScalar(0, 0, 0));
	double whiteDistance = distanceBetweenColors(color, cvScalar(255, 255, 255));
	return (blackDistance < whiteDistance) ? 8 : 16;
}

void ColorDetector::getBaseColors(CvScalar* resultColors) {
	int i;
	for (i = 0; i < mColorNum; ++i)
	{
		resultColors[i] = mColorMatrix.get(BASE_COLOR_POSITIONS[i]);
	}
}

CvScalar ColorDetector::selectColor(CvScalar color, CvScalar* baseColors) {
	int i, colorIndex;
	double *distance = (double*)malloc(sizeof(double)*mColorNum);
	for (i = 0; i < mColorNum; i++) {
		distance[i] = -(distanceBetweenColors(color, baseColors[i]));
	}
	colorIndex = get_max_index(distance, mColorNum);
	free(distance);
	switch (mColorNum) {
	case 8:
		return mOutputColor.getColorOf8Colors(colorIndex);
	case 16:
		return mOutputColor.getColorOf16Colors(colorIndex);
	default:
		return cvScalar(0, 0, 0);
	}
}

double ColorDetector::distanceBetweenColors(CvScalar color1, CvScalar color2) {
	double bDistance, gDistance, rDistance;
	bDistance = color1.val[0]-color2.val[0];
	gDistance = color1.val[1]-color2.val[1];
	rDistance = color1.val[2]-color2.val[2];
	return sqrt((bDistance*bDistance) + (gDistance*gDistance) + (rDistance*rDistance));
}
}
