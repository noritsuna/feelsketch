/*
 * ImageUtil.cpp
 *
 *  Created on: 2009/11/17
 *      Author: gabu
 */

#include "ImageUtil.h"

ImageUtil::ImageUtil() {
}

ImageUtil::~ImageUtil() {
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�P�s�̃r�b�g���v�Z										 //
// ����				:BitCount			:�r�b�g�J�E���g(8,16,24,32�̂�)		 //
//					:Width				:��									 //
// �߂�l			:�P�s�̃r�b�g��											 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
long ImageUtil::CalcPitch (short BitCount,long Width) {

	long lPitch;
	if (BitCount < BIT_COUNT_8) {
		int n = BIT_COUNT_8 / BitCount;
		lPitch = (Width + n - 1) / n;
	} else {
		lPitch = BitCount / 8;
		if (Width % 4 == 0) {
			return Width * lPitch;
		} else {
			return Width * lPitch + (4 - (Width * lPitch) % 4);
		}
	}
	return (lPitch + 3) &~3;
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�r�b�g�}�b�v�̊g�又��									 //
// ����				:lpBMP				:�摜�f�[�^							 //
//					:iAfterWidth		:�g�又���㕝						 //
//					:iAfterHeight		:�g�又���㍂��						 //
//					:iBeforeWidth		:�g�又���O��						 //
//					:iBeforeHeight		:�g�又���O����						 //
// �߂�l			:TRUE:�����@FALSE:���s									 //
// ���l				:�Q�S�r�b�g�J���[�̃r�b�g�}�b�v�̂ݑΉ�					 //
//					:�g�嗦�������ƂȂ�l�摜�̑傫���ɒ��ӂ���				 //
// ------------------------------------------------------------------------- //
BOOL ImageUtil::UpScale (LPBYTE *lpBMP, int iAfterWidth, int iAfterHeight, int iBeforeWidth, int iBeforeHeight) {

	int		iBeforeLength;
	int		iAfterLength;
	int		iMagnification;
	int		iBeforePos;
	int		iAfterPos;
	BYTE	*pbyTemp;
	BYTE	*pBMP;

	pBMP = *lpBMP;
	*lpBMP = NULL;
	if (pBMP == NULL) {														// �摜�o�b�t�@���m��
		return FALSE;
	}

	// �g��T�C�Y�����̃r�b�g�}�b�v�T�C�Y��菬�����ꍇ�̓G���[
	if (iAfterWidth < iBeforeWidth || iAfterHeight < iBeforeHeight) {
		return FALSE;
	}

	iBeforeLength = CalcPitch (BIT_COUNT_24, iBeforeWidth);
	iAfterLength = CalcPitch (BIT_COUNT_24, iAfterWidth);

//	�������̊g��
	if (iAfterWidth != iBeforeWidth) {										// �{������v���Ȃ����̂ݎ��{
		// ��Ɨ̈�̍Ċm�ۂ��s���A��Ɨ̈�ɃR�s�[
		pbyTemp = (LPBYTE)malloc (iBeforeLength * iBeforeHeight + 1);
		if (pbyTemp == NULL) {
			return FALSE;
		}
		memset (pbyTemp, '\0', iBeforeLength * iBeforeHeight + 1);
		memcpy (pbyTemp, pBMP, iBeforeLength * iBeforeHeight);

		// �摜�f�[�^�̈�̍Ċm�ۂ��s��
		free (pBMP);
		pBMP = NULL;
		pBMP = (LPBYTE)malloc (iAfterLength * iBeforeHeight);
		if (pBMP == NULL) {													// �̈�m�ێ��s
			free (pbyTemp);
			pbyTemp = NULL;
			return FALSE;
		}
		memset (pBMP, '\0', iAfterLength * iBeforeHeight);					// �̈�̏�����
		iMagnification = iAfterWidth / iBeforeWidth;						// �������̊g�嗦

		for (int i = 0; i < iBeforeHeight; i ++) {
			for (int j = 0; j < iBeforeWidth; j ++) {						// ���g���̍���
				iBeforePos	= (i * iBeforeLength) + (j * 3);
				for (int k = 0; k < iMagnification; k ++) {
					iAfterPos = (i * iAfterLength) + (j * 3 * iMagnification) + (k * 3);
					memcpy (&pBMP [iAfterPos], &pbyTemp [iBeforePos], 3);
				}
			}
		}
		free (pbyTemp);														// ��Ɨ̈�̊J��
		pbyTemp = NULL;
	}
	iBeforeLength = iAfterLength;

// �c�����̊g��
	if (iAfterHeight != iBeforeHeight) {									// �{������v���Ȃ����̂ݎ��{
		// ��Ɨ̈�̊m�ۂ��s���A��Ɨ̈�ɃR�s�[
		pbyTemp = (LPBYTE)malloc (iAfterLength * iBeforeHeight + 1);
		if (pbyTemp == NULL) {												// �̈�m�ێ��s
			return FALSE;
		}
		memset (pbyTemp, '\0', iAfterLength * iBeforeHeight + 1);
		memcpy (pbyTemp, pBMP, iAfterLength * iBeforeHeight);

		// �摜�f�[�^�̈�̍Ċm�ۂ��s��
		free (pBMP);
		pBMP = NULL;
		pBMP = (LPBYTE)malloc (iAfterLength * iAfterHeight + 1);			// �g����
		if (pBMP == NULL) {													// �̈�m�ێ��s
			free (pbyTemp);
			pbyTemp = NULL;
			return FALSE;
		}
		memset (pBMP, '\0', iAfterLength * iAfterHeight + 1);				// �̈�̏�����
		iMagnification = iAfterHeight / iBeforeHeight;						// �c�����̊g�嗦
		for (int i = 0; i < iAfterWidth; i ++) {
			for (int j = 0; j < iBeforeHeight; j ++) {						// �g���̍������ƂȂ�
				iBeforePos	= (j * iAfterLength) + (i * 3);
				for (int k = 0; k < iMagnification; k ++) {
					iAfterPos = (j * iAfterLength * iMagnification) + (k * iAfterLength) + (i * 3);
					memcpy (&pBMP [iAfterPos], &pbyTemp [iBeforePos], 3);
				}
			}
		}
		free (pbyTemp);														// ��Ɨ̈���J��
		pbyTemp = NULL;
	}
	*lpBMP = pBMP;
	return TRUE;
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�r�b�g�}�b�v�̏㉺����ւ�����							 //
// ����				:lpBMP				:�摜�f�[�^							 //
//					:iWidth				:��									 //
//					:iHeight			:����								 //
// �߂�l			:TRUE:�����@FALSE:���s									 //
// ���l				:�Q�S�r�b�g�J���[�̃r�b�g�}�b�v�̂ݑΉ�					 //
// ------------------------------------------------------------------------- //
BOOL ImageUtil::UpDownReplace (LPBYTE lpBMP, int iWidth, int iHeight) {

	int		iLength;
	BYTE	*pbyTemp;

	if (lpBMP == NULL) {													// �摜�o�b�t�@���m��
		return FALSE;
	}
	iLength = CalcPitch (BIT_COUNT_24, iWidth);

	// ��Ɨ̈�̍Ċm�ۂ��s���A��Ɨ̈�ɃR�s�[
	pbyTemp = (LPBYTE)malloc (iLength * iHeight + 1);
	if (pbyTemp == NULL) {
		return FALSE;
	}
	memset (pbyTemp, '\0', iLength * iHeight + 1);
	memcpy (pbyTemp, lpBMP, iLength * iHeight);
	memset (lpBMP, '\0', iLength * iHeight);								// �̈�̏�����

	for (int i = 0; i < iHeight; i ++) {
		memcpy (&lpBMP [i * iLength], &pbyTemp [(iHeight - (i + 1) )*iLength], iLength);
	}
	free (pbyTemp);															// ��Ɨ̈�̊J��
	pbyTemp = NULL;
	return TRUE;
}
