/*
 * PMCodeImageData.cpp
 *
 *  Created on: 2009/11/19
 *      Author: gabu
 */

#include "PMCodeImageData.h"

PMCodeImageData::PMCodeImageData() {
}

PMCodeImageData::~PMCodeImageData() {
}

void PMCodeImageData::setImage(char *image) {
	mImage = image;
}

char* PMCodeImageData::getImage() {
	return mImage;
}

void PMCodeImageData::setImageSize(UINT imageSize) {
	mImageSize = imageSize;
}

UINT PMCodeImageData::getImageSize() {
	return mImageSize;
}

void PMCodeImageData::setWidth(int width) {
	mWidth = width;
}

int PMCodeImageData::getWidth() {
	return mWidth;
}

void PMCodeImageData::setHeight(int height) {
	mHeight = height;
}

int PMCodeImageData::getHeight() {
	return mHeight;
}
