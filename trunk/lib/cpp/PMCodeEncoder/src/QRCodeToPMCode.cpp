#include "QRCodeToPMCode.h"
#include "global.h"
//#include "CtrlBMP.h"
#include "define.h"
#include <stdio.h>

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�R���X�g���N�^											 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
CQRCodeToPMCode::CQRCodeToPMCode () {

}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�f�X�g���N�^											 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
CQRCodeToPMCode::~CQRCodeToPMCode () {

}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�p�q�R�[�h���o�l�R�[�h�ϊ�								 //
// ����				:szQRCodeImage		:�p�q�R�[�h�摜�f�[�^				 //
//                  :uiQRCodeImageSize	:�p�q�R�[�h�摜�f�[�^�T�C�Y			 //
//					:lColorCode			:�J���[�R�[�h						 //
//					:szPMCodeImage		:�o�l�R�[�h�摜�f�[�^				 //
//                  :uiPMCodeImageSize	:�o�l�R�[�h�摜�f�[�^�T�C�Y			 //
//                  :uiSymbolSize		:�V���{���T�C�Y						 //
// �߂�l			:RESULT_OK												 //
// ���l				:�o�l�R�[�h�̃o�b�t�@�͗\�ߊm�ۂ��Ă�������				 //
//					:�����ɂēn���ꂽ�o�l�R�[�h�ɐF��ǉ����Ă����̂�		 //
//					:�o�b�t�@�̓N���A���Ȃ����I								 //
// ------------------------------------------------------------------------- //
int CQRCodeToPMCode::QRCodeToPMCode (char *szQRCodeImage, UINT uiQRCodeImageSize, long lColorCode
							, char *szPMCodeImage, UINT uiPMCodeImageSize, int uiSymbolSize) {

	int		iBaseImagePos;
	int		iValueImagePos;
	int		iBaseWidthLength;
	int		iValueWidthLength;
	int		iBitSize;
	char	szColor	[17];
	char	cTemp;

	iBitSize	= BIT_COUNT_24 / 8;

	iBaseWidthLength = CalcPitch (BIT_COUNT_8, uiSymbolSize);
	iValueWidthLength = CalcPitch (BIT_COUNT_24, uiSymbolSize);

	if (uiQRCodeImageSize < ((unsigned int)iBaseWidthLength * uiSymbolSize)) {
		return RESULT_ERROR_SECURE_MEMORY;	
	}
	if (uiPMCodeImageSize < ((unsigned int)iValueWidthLength * uiSymbolSize)) {
		return RESULT_ERROR_SECURE_MEMORY;	
	}
	memset (szColor, '\0', sizeof (szColor));
	sprintf (szColor, "%0.6X", lColorCode);

	cTemp = szColor [0];
	szColor [0] = szColor [4];
	szColor [4] = cTemp;
	cTemp = szColor [1];
	szColor [1] = szColor [5];
	szColor [5] = cTemp;

	// �d�􏈗�
	for (int i = 0; i < uiSymbolSize; i ++) {
		for (int j = 0; j < uiSymbolSize; j ++) {
			// �p�q�R�[�h�摜�̌��o�ʒu�Z�o
			iBaseImagePos = (i * iBaseWidthLength) + j;

			// �o�l�R�[�h�摜�̐ݒ�ʒu�Z�o
			iValueImagePos = (i * iValueWidthLength) + (j * iBitSize);

			if ((szQRCodeImage [iBaseImagePos] & BIT_ON) == BIT_ON) {		// ��
				szPMCodeImage [iValueImagePos] = szPMCodeImage [iValueImagePos] | h2tb1 (&szColor [0]);
				szPMCodeImage [iValueImagePos + 1] = szPMCodeImage [iValueImagePos + 1] | h2tb1 (&szColor [2]);
				szPMCodeImage [iValueImagePos + 2] = szPMCodeImage [iValueImagePos + 2] | h2tb1 (&szColor [4]);
			}
		}
	}
	return RESULT_OK;
}
