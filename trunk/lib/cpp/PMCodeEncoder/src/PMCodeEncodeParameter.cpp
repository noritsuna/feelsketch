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
