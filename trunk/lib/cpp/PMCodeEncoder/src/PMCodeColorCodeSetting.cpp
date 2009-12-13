/*
 * PMCodeColorCodeSetting.cpp
 *
 *  Created on: 2009/11/18
 *      Author: gabu
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
