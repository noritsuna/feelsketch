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

#ifndef RECTCALCULATOR_H_
#define RECTCALCULATOR_H_

namespace feelsketch {

class RectCalculator {
public:
	typedef enum {
		TOP_LEFT=1,
		CENTER_LEFT,
		BOTTOM_LEFT,
		TOP_RIGHT,
		CENTER_RIGHT,
		BOTTOM_RIGHT,
		TOP_CENTER,
		BOTTOM_CENTER,
		CENTER_CENTER
	}PMCodePos;
	RectCalculator();
	virtual ~RectCalculator();
	void calculate(int* resultRect, int* pmCodeRect, int pmCodeSize, PMCodePos pmCodePos, int paperSize, int imageWidth, int imageHeight);
private:
	static const float PAPER_ASPECT[8];
	int getLeft(int* pmCodeRect, PMCodePos pmCodePos, int width);
	int getTop(int* pmCodeRect, PMCodePos pmCodePos, int height);
};

}

#endif /* RECTCALCULATOR_H_ */
