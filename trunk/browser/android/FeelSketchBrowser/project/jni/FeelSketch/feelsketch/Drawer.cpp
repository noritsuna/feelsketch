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

#include "Drawer.h"
#include <fstream>
#include <stdio.h>
#include "../util.h"

namespace feelsketch {
using namespace std;

Drawer::Drawer(IplImage* image) :
mImage(image) {
}

Drawer::~Drawer() {
}

void Drawer::drawLine(CvPoint start, CvPoint end, CvScalar color, int thickness) {
	cvLine(mImage, start, end, color, thickness);
}

void Drawer::drawBezier(CvPoint* points, CvScalar color, int thickness)
{
	int i, tmpX, tmpY;
	int x = points[0].x , y = points[0].y;

	for(double t = 0; t <= 1;t += 0.005)
	{
		tmpX = x;
		tmpY = y;

		x = int(pow(1-t, 3) * points[0].x + 3 * pow(1-t,2) * t * points[1].x + 3*(1-t) * t*t * points[2].x + t*t*t * points[3].x);
		y = int(pow(1-t, 3) * points[0].y + 3 * pow(1-t,2) * t * points[1].y + 3*(1-t) * t*t * points[2].y + t*t*t * points[3].y);

		cvLine(mImage,cvPoint(tmpX,tmpY),cvPoint(x,y), color, thickness);

	}

}

void Drawer::drawIcon(CvPoint center, int icon_id)
{
	int x, y, width, height, left, top;
    char fname[128];
    char imageData[100*100*3];
    width = 100;
    height = 100;
    sprintf(fname, "/data/data/jp.co.cia.feelsketch/files/%d", icon_id);
    ifstream f;
    f.open(fname, ios::in | ios::binary);
    f.read(imageData, width*height*3);
    f.close();
    IplImage *icon = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
    icon->imageData = imageData;
    left = center.x-(width/2);
    top  = center.y-(height/2);
	for ( y = 0; y < height; y++ ) {
        for ( x = 0; x < width; x++ ) {
        	// ignore transparent (bgr = (1, 1, 1))
        	if (!(IMAGE(icon, x, y, 0) == 1 && IMAGE(icon, x, y, 1) == 1 && IMAGE(icon, x, y, 2) == 1)
        			&& x+left < mImage->width && y+top < mImage->height) {
    			IMAGE(mImage, x+left, y+top, 0) = IMAGE(icon, x, y, 0);
    			IMAGE(mImage, x+left, y+top, 1) = IMAGE(icon, x, y, 1);
    			IMAGE(mImage, x+left, y+top, 2) = IMAGE(icon, x, y, 2);
        	}
        }
	}
}
}
