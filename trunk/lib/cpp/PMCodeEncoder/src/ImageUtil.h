/*
 * ImageUtil.h
 *
 *  Created on: 2009/11/17
 *      Author: gabu
 */

#ifndef IMAGEUTIL_H_
#define IMAGEUTIL_H_

#include <string.h>
#include <stdlib.h>

#include "define.h"

class ImageUtil {
public:
	ImageUtil();
	virtual ~ImageUtil();
	static long ImageUtil::CalcPitch (short BitCount,long Width);
	static BOOL UpScale (LPBYTE *lpBMP, int iAfterWidth, int iAfterHeight, int iBeforeWidth, int iBeforeHeight);
	static BOOL UpDownReplace (LPBYTE lpBMP, int iWidth, int iHeight);
};

#endif /* IMAGEUTIL_H_ */
