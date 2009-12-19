/*
 * Parser.h
 *
 *  Created on: 2009/11/19
 *      Author: yukou
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "Drawer.h"

namespace feelsketch {
using namespace std;

struct FSResult {
	int version;
	int pmcodeSize;
	int pmcodePos;
	int paperSize;
	int bgCode;
	int reserved;
	char uuid[16];
	char mediaData[1024];
	int* imageData;
};

class Parser {
private:
	typedef enum {
		LAYER=0,
		ICON,
		LINE,
		BEZIER,
		SPLINE,
		LETTER,
		SHAPE,
		TERMINAL=255
	}Command;
	static const CvScalar COLORS[256];
	static const int LINE_THICKNESS[10];
	char* mFSData;
	int mFSDataSize;
	int mImageWidth;
	int mImageHeight;
	int parseHeader(FSResult* fsResult);
	int parseBody(FSResult* fsResult);
	int* pixelsFromIplImage(IplImage* image);
	CvPoint calculatePoint(int x, int y);
	CvScalar getColor(int colorCode, int lineType);
public:
	Parser(char* fsData, int fsDataSize, int imageWidth, int imageHeight);
	virtual ~Parser();
	int parse(FSResult* fsResult);
};

}

#endif /* PARSER_H_ */
