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

#include "OutputColor.h"

namespace pmcode {
OutputColor::OutputColor(BaseColor baseColor) {
	create8Colors(baseColor);
	create16Colors(baseColor);
}

OutputColor::~OutputColor() {
}

CvScalar OutputColor::getColorOf8Colors(int index) {
	return m8Colors[index];
}

CvScalar OutputColor::getColorOf16Colors(int index) {
	return m16Colors[index];
}

// private

// sizeof(composedColors) >= 2 ^ layerCount - 1
void OutputColor::composeColor(int layerCount, int* baseColors, int* composedColors) {
	int i, j, iColorCount = layerCount, idiff = 0, iBits = 0;
	for (i = 0; i < layerCount; i++) composedColors[i] = baseColors[i];
	int iColorCompound = pow(2, layerCount) - 1;
	for (int i = 1; i <= iColorCompound; i ++){
		if ((0x01 << idiff) == i) {
			idiff ++;
		} else {
			for (int j = 0; j < layerCount; j ++) {
				iBits = ((i >> j) & 0x01);
				if (iBits == 0x01) {
					composedColors[iColorCount] += baseColors[j];
				}
			}
			iColorCount ++;
		}
	}
}

CvScalar OutputColor::cvScalarInt(int i) {
    int mask = 0x000000FF;
    int r = i >> 16 & mask;
    int g = i >> 8 & mask;
    int b = i & mask;
    return cvScalar(b, g, r);
}

void OutputColor::create8Colors(BaseColor baseColor) {
	int i;
	int baseColors[3], composedColors[7] = {0};
	for (i = 0; i < 3; i++) baseColors[i] = baseColor.getColorOf3Layers(i);
	composeColor(3, baseColors, composedColors);
	for (i = 0; i < 7; i++) m8Colors[i] = cvScalarInt(composedColors[i]);
	m8Colors[7] = cvScalar(0, 0, 0); // black is constant.
}

void OutputColor::create16Colors(BaseColor baseColor) {
	int i;
	int baseColors[4], composedColors[15] = {0};
	for (i = 0; i < 4; i++) baseColors[i] = baseColor.getColorOf4Layers(i);
	composeColor(4, baseColors, composedColors);
	for (i = 0; i < 15; i++) m16Colors[i] = cvScalarInt(composedColors[i]);
	m16Colors[15] = cvScalar(0, 0, 0); // black is constant.
}
}
