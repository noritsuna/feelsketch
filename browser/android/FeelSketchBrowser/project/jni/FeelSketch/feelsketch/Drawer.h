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

#ifndef DRAWER_H_
#define DRAWER_H_

#include <cv.h>

namespace feelsketch {
class Drawer {
private:
	IplImage* mImage;
	void solveSpline(int n, CvPoint* points, CvMat* dst);
	float funcSpline(int n, float x, const CvPoint* points, const CvMat* c_mat);
public:
	Drawer(IplImage* image);
	virtual ~Drawer();
	void drawLine(CvPoint start, CvPoint end, CvScalar color, int thickness);
	void drawBezier(CvPoint* points, CvScalar color, int thickness);
	void drawSpline(CvPoint* points, int points_num, CvScalar color, int thickness);
	void drawIcon(CvPoint center, int icon_id);
};
}
#endif /* DRAWER_H_ */
