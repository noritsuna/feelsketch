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

#include "PMCodeEncoder.h"

PMCodeEncoder::PMCodeEncoder() {
	mCPMCodeEncode = CPMCodeEncode();
}

PMCodeEncoder::~PMCodeEncoder() {
}

int PMCodeEncoder::encode(PMCodeEncodeParameter param, PMCodeImageData &result) {
	int i = 0, ret = 1;

	PMCodeColorCodeSetting colorCode = param.getPMCodeColorCodeSetting();
	for (i = 0; i < param.getLayer(); i++) {
		g_ColorCodeSettingDefinition[i] = colorCode.getColorCode(i);
	}

	MakeColorTable(param.getLayer(), param.getLayer());

	ret = check(param);
	if (ret != TRUE) return ret;

	ret = mCPMCodeEncode.SetPMCodeData(param.getData(),
			              param.getDataSize(),
			              param.getLayer(),
			              param.getVersion(),
			              param.getErrorCorrection(),
			              param.getMaskAuto(),
			              param.getModuleSize(),
			              param.getExtension(),
			              param.getNote());
	if (ret != RESULT_OK) return ret;

	ret = mCPMCodeEncode.EncodePMCode();
	if (ret != RESULT_OK) return ret;

	char	*image;
	UINT	imageSize;
	int		width, height;
	ret = mCPMCodeEncode.GetPMCodeImageData(&image, &imageSize, &width, &height);
	if (ret != RESULT_OK) return ret;
	result.setImage(image);
	result.setImageSize(imageSize);
	result.setWidth(width);
	result.setHeight(height);

	return RESULT_OK;
}

BOOL PMCodeEncoder::check(PMCodeEncodeParameter &param) {

	BOOL	bRet;
	int		iMinVersion;
	UINT    dataSize = param.getDataSize() + EXT_SIZE + SIZE_SIZE + RESERVE_SIZE;

	if (param.getVersion() == VERSION_AUTO) {
		if (param.getLayer() == 3) {
			iMinVersion = MIN_VERSIONSIZE;
		} else {
			iMinVersion = 3;
		}
		for (int i = iMinVersion; i <= MAX_VERSIONSIZE; i ++) {
			bRet = mCPMCodeEncode.CheckDataSizeToPMCode (dataSize, param.getLayer(), i, param.getErrorCorrection());
			if (bRet == TRUE){
				param.setVersion(i);
				return TRUE;
			}
		}
	}
	bRet = mCPMCodeEncode.CheckDataSizeToPMCode (dataSize, param.getLayer(), param.getVersion(), param.getErrorCorrection());
	if (bRet == TRUE){
		return TRUE;
	}
	return FALSE;
}
