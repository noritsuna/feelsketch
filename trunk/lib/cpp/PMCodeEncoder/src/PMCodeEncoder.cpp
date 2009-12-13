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

	// ��F�̐ݒ�
	PMCodeColorCodeSetting colorCode = param.getPMCodeColorCodeSetting();
	for (i = 0; i < param.getLayer(); i++) {
		g_ColorCodeSettingDefinition[i] = colorCode.getColorCode(i);
	}

	// �`��F�̐���
	MakeColorTable(param.getLayer(), param.getLayer()); // 1�w1�F�Ȃ̂�

	// �f�[�^���w�肳�ꂽ�^�ԂŎ��܂邩�`�F�b�N
	// Auto�̏ꍇ�́A�����ōœK�Ȍ^�Ԃ����肷��B
	ret = check(param);
	if (ret != TRUE) return ret;

	// �G���R�[�h�������f�[�^���Z�b�g
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

	// �G���R�[�h�I
	ret = mCPMCodeEncode.EncodePMCode();
	if (ret != RESULT_OK) return ret;

	// ���ʂ��擾���āA�l�ߍ���
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

// �w�肳�ꂽ�^�Ԃło�l�R�[�h�̍쐬���ł��邩�`�F�b�N����B
// �^�Ԃ������̏ꍇ�A�œK�Ȍ^�Ԃ��Z�b�g����B
BOOL PMCodeEncoder::check(PMCodeEncodeParameter &param) {

	BOOL	bRet;
	int		iMinVersion;
	UINT    dataSize = param.getDataSize() + EXT_SIZE + SIZE_SIZE + RESERVE_SIZE;

	// �^�Ԃ������̂Ƃ�
	if (param.getVersion() == VERSION_AUTO) {
		// 2006/11/27 �F���ʃR�[�h�̊g��(�U�w�̂�)
		if (param.getLayer() == 3) {
			iMinVersion = MIN_VERSIONSIZE;
		} else {
			iMinVersion = 3;												// �w�����U�̏ꍇ�A�Œ�^�Ԃ͂R
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
