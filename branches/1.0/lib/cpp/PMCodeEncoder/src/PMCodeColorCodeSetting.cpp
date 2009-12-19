/**
***                  "Feel Sketch" PMCode Encoder & Decoder.
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

#include "PMCodeColorCodeSetting.h"

PMCodeColorCodeSetting::PMCodeColorCodeSetting() {
	// set default color code.
	setColorCode(0, 0x0000FF);
	setColorCode(1, 0x00FF00);
	setColorCode(2, 0xFF0000);
}

PMCodeColorCodeSetting::~PMCodeColorCodeSetting() {
}

void PMCodeColorCodeSetting::setColorCode(int layer, long value) {
	if (layer >= MAX_LAYERSIZE) return;
	mColorCodes[layer] = value;
}

long PMCodeColorCodeSetting::getColorCode(int layer) {
	if (layer >= MAX_LAYERSIZE) return 0;
	return mColorCodes[layer];
}
