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

#include "Parser.h"
#include <string>
#include <sstream>
#include "../util.h"

namespace feelsketch {
using namespace std;

const CvScalar Parser::COLORS[256] = {
		cvScalar(  1,   1,   1), // transparent
		cvScalar( 53,  96, 124), // brown
		cvScalar(  0,   0, 204), // red
		cvScalar( 76, 183, 255), // orange
		cvScalar(  0, 255, 255), // yellow
		cvScalar( 65, 255,   0), // green
		cvScalar(204,   0,  12), // blue
		cvScalar(204,   0, 196), // purple
		cvScalar(140, 140, 140), // grey
		cvScalar(250, 250, 250), // white
		cvScalar( 38,  38,  38), // black
		cvScalar(173, 210, 234), // skin
		cvScalar(166, 171, 248)  // pink
};
const int Parser::LINE_THICKNESS[10] = {
		 0,
		 2, // 1*480/255=1.88
		 6, // 3*480/255=5.64
		11, // 6*480/255=11.29
		19, // 10*480/255=18.82
		 6,
		11,
		19
};

Parser::Parser(char* fsData, int fsDataSize, int imageWidth, int imageHeight) :
mFSData(fsData), mFSDataSize(fsDataSize), mImageWidth(imageWidth), mImageHeight(imageHeight) {
}

Parser::~Parser() {
}

int Parser::parse(FSResult* fsResult) {
	int error;

	if (error = parseHeader(fsResult) != 0) return error;
	if (error = parseBody(fsResult) != 0) return error;

	return 0;
}

int Parser::parseHeader(FSResult* fsResult) {
	string str(mFSData, mFSDataSize);
	string mediaStr;
	char c;
	istringstream fsDataStream(str);

	fsDataStream.seekg(0, ios_base::beg);
	fsResult->version    = fsDataStream.get();			// version
	fsDataStream.get(); fsDataStream.get();				// image size
	fsResult->pmcodeSize = fsDataStream.get();			// PMCode size
	fsResult->pmcodePos  = fsDataStream.get();			// PMCode position
	fsResult->paperSize  = fsDataStream.get();			// paper size
	fsResult->bgCode     = fsDataStream.get();			// background code
	fsResult->reserved   = fsDataStream.get();			// reserved
	fsDataStream.read(fsResult->uuid, 16);				// UUID
	fsDataStream.getline(fsResult->mediaData, 1024, '\r');	// media data

	LOG_FUNC_MESSAGE(fsResult->mediaData);

	return 0;
}

int Parser::parseBody(FSResult* fsResult) {
	LOG_FUNC_START;
	string str(mFSData, mFSDataSize);
	istringstream fsDataStream(str);
	char dummy[1024];
	char msg[128];

    CvSize imageSize = cvSize(mImageWidth, mImageHeight);
	IplImage* image = cvCreateImage(imageSize, IPL_DEPTH_8U, 3);
	cvSet(image, COLORS[0], 0);
	Drawer drawer(image);

	// skip header
	fsDataStream.seekg(24, ios_base::beg);
	fsDataStream.get(dummy, 1024, '\n');
	fsDataStream.get();

	LOG_FUNC_MESSAGE("parse body");

	Command c;
	int i, j, x, y, id, effect, colorCode, lineType;
	CvScalar color;
	CvPoint p1, p2, b[4];
	while (fsDataStream.good()) {
		c = (Command)fsDataStream.get();
		sprintf(msg, "command = %d", (int)c);
		LOG_FUNC_MESSAGE(msg);
		switch (c) {
			case LAYER:
				fsDataStream.get();		// layer pos
				effect = (fsDataStream.get() << 8) | fsDataStream.get(); // effect
				sprintf(msg, "effect = %d", effect);
				LOG_FUNC_MESSAGE(msg);
				break;
			case ICON:
				x = fsDataStream.get();
				y = fsDataStream.get();
				id = (fsDataStream.get() << 8) | fsDataStream.get();
				p1 = calculatePoint(x, y);
				drawer.drawIcon(p1, id);
				if (fsDataStream.get() != TERMINAL) return -1;
				if (fsDataStream.get() != TERMINAL) return -1;
				break;
			case LINE:
				lineType = fsDataStream.get();
				colorCode = fsDataStream.get();
				color = getColor(colorCode, lineType);
				x = fsDataStream.get();
				y = fsDataStream.get();
				p1 = calculatePoint(x, y);
				x = fsDataStream.get();
				y = fsDataStream.get();
				p2 = calculatePoint(x, y);
				drawer.drawLine(p1, p2, color, LINE_THICKNESS[lineType]);
				while(1) {
					if ((x = fsDataStream.get()) != TERMINAL) {
						p1 = p2;
						y = fsDataStream.get();
						p2 = calculatePoint(x, y);
						drawer.drawLine(p1, p2, color, LINE_THICKNESS[lineType]);
					} else {
						if (fsDataStream.get() == TERMINAL) {
							break;
						} else {
							return -1;
						}
					}
				}
				break;
			case BEZIER:
				lineType = fsDataStream.get();
				colorCode = fsDataStream.get();
				color = getColor(colorCode, lineType);
				for (i = 0; i < 4; i++) {
					x = fsDataStream.get();
					y = fsDataStream.get();
					b[i] = calculatePoint(x, y);
				}
				sprintf(msg, "color = %d", colorCode);
				LOG_FUNC_MESSAGE(msg);
				sprintf(msg, "bezier = { (%d, %d), (%d, %d), (%d, %d), (%d, %d) }", b[0].x, b[0].y, b[1].x, b[1].y, b[2].x, b[2].y, b[3].x, b[3].y);
				LOG_FUNC_MESSAGE(msg);
				drawer.drawBezier(b, color, LINE_THICKNESS[lineType]);
				while(1) {
					if ((x = fsDataStream.get()) != TERMINAL) {
						b[0] = b[3];
						y = fsDataStream.get();
						b[1] = calculatePoint(x, y);
						for (i = 2; i < 4; i++) {
							x = fsDataStream.get();
							y = fsDataStream.get();
							b[i] = calculatePoint(x, y);
						}
						sprintf(msg, "bezier = { (%d, %d), (%d, %d), (%d, %d), (%d, %d) }", b[0].x, b[0].y, b[1].x, b[1].y, b[2].x, b[2].y, b[3].x, b[3].y);
						LOG_FUNC_MESSAGE(msg);
						drawer.drawBezier(b, color, LINE_THICKNESS[lineType]);
					} else {
						if (fsDataStream.get() == TERMINAL) {
							break;
						} else {
							return -1;
						}
					}
				}
				break;
			case SPLINE:
				while(1) {
					if (fsDataStream.get() == TERMINAL) {
						if (fsDataStream.get() == TERMINAL) {
							break;
						} else {
							return -1;
						}
					}
				}
				break;
			default:
				break;
		}
	}

	fsResult->imageData[0] = pixelsFromIplImage(image);
	fsResult->imageData[1] = pixelsFromIplImage(image);
	fsResult->imageData[2] = pixelsFromIplImage(image);
	fsResult->imageData[3] = pixelsFromIplImage(image);
    cvReleaseImage(& image);

    return 0;
}

int* Parser::pixelsFromIplImage(IplImage* image) {
	int x, y;
	int* pixels = (int*)malloc(sizeof(int)*image->width*image->height);

	for ( y = 0; y < image->height; y++ ) {
        for ( x = 0; x < image->width; x++ ) {
        	if (IMAGE(image, x, y, 0) == 1 && IMAGE(image, x, y, 1) == 1 && IMAGE(image, x, y, 2) == 1) {
            	// bgr = (1, 1, 1) = transparent
    			pixels[x+y*image->width] = 0;
        	} else if (IMAGE(image, x, y, 0) == 2 && IMAGE(image, x, y, 1) == 2) {
            	// bgr = (2, 2, colorCode) = translucent
        		pixels[x+y*image->width] = (0xAA << 24) | ((int)(COLORS[IMAGE(image, x, y, 2)].val[2]) << 16) | ((int)(COLORS[IMAGE(image, x, y, 2)].val[1]) << 8) | (int)COLORS[IMAGE(image, x, y, 2)].val[0];
        	} else {
        		pixels[x+y*image->width] = (0xFF << 24) | (IMAGE(image, x, y, 2) << 16) | (IMAGE(image, x, y, 1) << 8) | IMAGE(image, x, y, 0);
        	}
        }
    }

	return pixels;
}

CvPoint Parser::calculatePoint(int x, int y) {
	return cvPoint((int)(x*mImageWidth/255.0), (int)(y*mImageHeight/255.0));
}

CvScalar Parser::getColor(int colorCode, int lineType) {
	if (lineType < 5) {
		return COLORS[colorCode];
	} else {
		// highlighter pen -> translucent
		return cvScalar(2, 2, colorCode);
	}
}

}
