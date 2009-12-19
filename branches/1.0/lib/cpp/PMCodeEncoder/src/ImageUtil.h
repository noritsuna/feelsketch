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
