/*
 * PMCodeEncodeParameter.cpp
 *
 *  Created on: 2009/11/18
 *      Author: gabu
 */

#include "PMCodeEncodeParameter.h"

PMCodeEncodeParameter::PMCodeEncodeParameter() {
	setPMCodeColorCodeSetting(PMCodeColorCodeSetting());
	setLayer(3);
	setVersion(0); // 0 is Auto
	setErrorCorrection(0);
	setMaskAuto(TRUE);
	setModuleSize(4);
	setExtension(NULL);
	setNote(NULL);
}

PMCodeEncodeParameter::~PMCodeEncodeParameter() {
}

void PMCodeEncodeParameter::setPMCodeColorCodeSetting(PMCodeColorCodeSetting pmCodeColorCodeSetting) {
	mPMCodeColorCodeSetting = pmCodeColorCodeSetting;
}
PMCodeColorCodeSetting PMCodeEncodeParameter::getPMCodeColorCodeSetting() {
	return mPMCodeColorCodeSetting;
}

void PMCodeEncodeParameter::setData(char *data) {
	mData = data;
}

char* PMCodeEncodeParameter::getData() {
	return mData;
}

void PMCodeEncodeParameter::setDataSize(UINT dataSize) {
	mDataSize = dataSize;
}

UINT PMCodeEncodeParameter::getDataSize() {
	return mDataSize;
}

void PMCodeEncodeParameter::setLayer(int layer) {
	mLayer = layer;
}

int PMCodeEncodeParameter::getLayer() {
	return mLayer;
}

void PMCodeEncodeParameter::setVersion(int version) {
	mVersion = version;
}

int PMCodeEncodeParameter::getVersion() {
	return mVersion;
}

void PMCodeEncodeParameter::setErrorCorrection(int errorCorrection) {
	mErrorCorrection = errorCorrection;
}

int PMCodeEncodeParameter::getErrorCorrection() {
	return mErrorCorrection;
}

void PMCodeEncodeParameter::setMaskAuto(BOOL maskAuto) {
	mMaskAuto = maskAuto;
}

BOOL PMCodeEncodeParameter::getMaskAuto() {
	return mMaskAuto;
}

void PMCodeEncodeParameter::setModuleSize(int moduleSize) {
	mModuleSize = moduleSize;
}

int PMCodeEncodeParameter::getModuleSize() {
	return mModuleSize;
}

void PMCodeEncodeParameter::setExtension(char *extension) {
	mExtension = extension;
}

char* PMCodeEncodeParameter::getExtension() {
	return mExtension;
}

void PMCodeEncodeParameter::setNote(char *note) {
	mNote = note;
}

char* PMCodeEncodeParameter::getNote() {
	return mNote;
}
