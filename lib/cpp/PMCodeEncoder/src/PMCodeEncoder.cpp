/*
 * PMCodeEncoder.cpp
 *
 *  Created on: 2009/11/19
 *      Author: gabu
 */

#include "PMCodeEncoder.h"

PMCodeEncoder::PMCodeEncoder() {
	mCPMCodeEncode = CPMCodeEncode();
}

PMCodeEncoder::~PMCodeEncoder() {
}

int PMCodeEncoder::encode(PMCodeEncodeParameter param, PMCodeImageData &result) {
	int i = 0, ret = 1;

	// 基準色の設定
	PMCodeColorCodeSetting colorCode = param.getPMCodeColorCodeSetting();
	for (i = 0; i < param.getLayer(); i++) {
		g_ColorCodeSettingDefinition[i] = colorCode.getColorCode(i);
	}

	// 描画色の生成
	MakeColorTable(param.getLayer(), param.getLayer()); // 1層1色なので

	// データが指定された型番で収まるかチェック
	// Autoの場合は、ここで最適な型番を決定する。
	ret = check(param);
	if (ret != TRUE) return ret;

	// エンコードしたいデータをセット
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

	// エンコード！
	ret = mCPMCodeEncode.EncodePMCode();
	if (ret != RESULT_OK) return ret;

	// 結果を取得して、詰め込む
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

// 指定された型番でＰＭコードの作成ができるかチェックする。
// 型番が自動の場合、最適な型番をセットする。
BOOL PMCodeEncoder::check(PMCodeEncodeParameter &param) {

	BOOL	bRet;
	int		iMinVersion;
	UINT    dataSize = param.getDataSize() + EXT_SIZE + SIZE_SIZE + RESERVE_SIZE;

	// 型番が自動のとき
	if (param.getVersion() == VERSION_AUTO) {
		// 2006/11/27 色識別コードの拡張(６層のみ)
		if (param.getLayer() == 3) {
			iMinVersion = MIN_VERSIONSIZE;
		} else {
			iMinVersion = 3;												// 層数が６の場合、最低型番は３
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
