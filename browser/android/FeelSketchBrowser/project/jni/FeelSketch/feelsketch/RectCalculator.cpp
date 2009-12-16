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

#include "RectCalculator.h"

namespace feelsketch {

const float RectCalculator::PAPER_ASPECT[8] = {
		0,
		1.34, // L
		1.48, // Card
		1.48, // Post Card
		1.41, // A3
		1.41, // A4
		1.41, // B4
		1.41  // B5
};

RectCalculator::RectCalculator() {
}

RectCalculator::~RectCalculator() {
}

void RectCalculator::calculate(int* resultRect, int* pmCodeRect, int pmCodeSize, PMCodePos pmCodePos, int paperSize, int imageWidth, int imageHeight) {
	int pmCodeWidth, pmCodeHeight, resultWidth, resultHeight;

	resultRect[0] = 0;
	resultRect[1] = 0;
	resultRect[2] = 0;
	resultRect[3] = 0;

	pmCodeWidth  = pmCodeRect[2] - pmCodeRect[0];
	pmCodeHeight = pmCodeRect[3] - pmCodeRect[1];
	if (pmCodeWidth <= 0 || pmCodeHeight <= 0) return;

	resultWidth  = pmCodeWidth * pmCodeSize * PAPER_ASPECT[paperSize];
	resultHeight = pmCodeHeight * pmCodeSize;
	if (resultWidth > imageWidth || resultHeight > imageHeight) return;

	resultRect[0] = getLeft(pmCodeRect, pmCodePos, resultWidth);
	resultRect[1] = getTop(pmCodeRect, pmCodePos, resultHeight);
	resultRect[2] = resultRect[0] + resultWidth;
	resultRect[3] = resultRect[1] + resultHeight;
	if (resultRect[0] < 0 || resultRect[1] < 0 ||resultRect[2] > imageWidth || resultRect[3] > imageHeight) {
		resultRect[0] = 0;
		resultRect[1] = 0;
		resultRect[2] = 0;
		resultRect[3] = 0;
	}

	return;
}

int RectCalculator::getLeft(int* pmCodeRect, PMCodePos pmCodePos, int width) {
	switch (pmCodePos) {
	case TOP_LEFT:
	case BOTTOM_LEFT:
	case CENTER_LEFT:
		return pmCodeRect[0];
	case TOP_RIGHT:
	case BOTTOM_RIGHT:
	case CENTER_RIGHT:
		return pmCodeRect[2] - width;
	case TOP_CENTER:
	case BOTTOM_CENTER:
	case CENTER_CENTER:
		return pmCodeRect[2] - (width / 2);
	}

}

int RectCalculator::getTop(int* pmCodeRect, PMCodePos pmCodePos, int height) {
	switch (pmCodePos) {
	case TOP_LEFT:
	case TOP_RIGHT:
	case TOP_CENTER:
		return pmCodeRect[1];
	case BOTTOM_LEFT:
	case BOTTOM_RIGHT:
	case BOTTOM_CENTER:
		return pmCodeRect[3] - height;
	case CENTER_LEFT:
	case CENTER_RIGHT:
	case CENTER_CENTER:
		return pmCodeRect[3] - (height / 2);
	}
}

}
