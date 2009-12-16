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

#ifndef __LEVEL_ADJUSTMENT_H__
#define __LEVEL_ADJUSTMENT_H__

#include "cxcore.h"

namespace pmcode {
class LevelAdjustment {
public:
	LevelAdjustment() { mIsPreparedLut = false; }
	~LevelAdjustment() { }
	CvScalar adjustColor(CvScalar color);
	void prepareLut(CvScalar black, CvScalar white, CvScalar grey);
private:
	int mLut[3][256]; // look up table
	bool mIsPreparedLut;
	void getLevelAjustmentLut(double black, double white, double grey, int* resultLut);
};
}

#endif /* __LEVEL_ADJUSTMENT_H__ */
