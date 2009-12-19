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

#include "LevelAdjustment.h"

namespace pmcode {
CvScalar LevelAdjustment::adjustColor(CvScalar color) {
	if (!mIsPreparedLut) return color;
	CvScalar result;
	result.val[0] = mLut[0][(int)color.val[0]];
	result.val[1] = mLut[1][(int)color.val[1]];
	result.val[2] = mLut[2][(int)color.val[2]];
	return result;
}

void LevelAdjustment::prepareLut(CvScalar black, CvScalar white, CvScalar grey) {
	getLevelAjustmentLut(black.val[0], white.val[0], grey.val[0], mLut[0]);
	getLevelAjustmentLut(black.val[1], white.val[1], grey.val[1], mLut[1]);
	getLevelAjustmentLut(black.val[2], white.val[2], grey.val[2], mLut[2]);
	mIsPreparedLut = true;
}

void LevelAdjustment::getLevelAjustmentLut(double black, double white, double grey, int* resultLut) {
	int i, tmp;
	double gamma = log((grey-black) / (white-black)) / log(0.5);
	for(i = 0; i < 256; i++)
	{
		if (i <= black) {
			tmp = 0;
		} else if (i >= white) {
			tmp = 255;
		} else {
			tmp = (int)(255.0 * pow(((double)i-black) / (white-black), 1.0 / gamma));
		}
		resultLut[i] = tmp;
	}
}
}
