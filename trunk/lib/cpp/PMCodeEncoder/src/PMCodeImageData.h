/*
 * PMCodeImageData.h
 *
 *  Created on: 2009/11/19
 *      Author: gabu
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
