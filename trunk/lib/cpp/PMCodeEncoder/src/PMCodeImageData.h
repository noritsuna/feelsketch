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

#ifndef PMCODEIMAGEDATA_H_
#define PMCODEIMAGEDATA_H_

#include "define.h"

class PMCodeImageData {
public:
	PMCodeImageData();
	virtual ~PMCodeImageData();

	// Setter/Getter
	void setImage(char *image);
	char* getImage();
	void setImageSize(UINT imageSize);
	UINT getImageSize();
	void setWidth(int width);
	int getWidth();
	void setHeight(int height);
	int getHeight();

private:
	char	*mImage;
	UINT	mImageSize;
	int		mWidth;
	int		mHeight;
};

#endif /* PMCODEIMAGEDATA_H_ */
