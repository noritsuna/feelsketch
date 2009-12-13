#include "define.h"
#include "PMCodeMakeHeader.h"
#include "global.h"
//#include "CtrlBMP.h"
#include <stdio.h>
// ------------------------------------------------------------------------- //
// �{�֐��ɓn�����o�l�R�[�h�摜�͕␳��̉摜�Ƃ��܂��B
// 
// ------------------------------------------------------------------------- //
#define QR_CODE_BIT_ON		'\x00'				// �r�b�g�n�m�i���j
#define QR_CODE_BIT_OFF		'\xFF'				// �r�b�g�n�e�e�i���j

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�R���X�g���N�^											 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
CPMCodeMakeHeader::CPMCodeMakeHeader () {

}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�f�X�g���N�^											 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
CPMCodeMakeHeader::~CPMCodeMakeHeader () {

}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�o�l�R�[�h�摜�̃Z�b�g									 //
// ����				:szImage		:�o�l�R�[�h�摜�f�[�^					 //
//                  :iImageSize		:�o�l�R�[�h�摜�f�[�^�T�C�Y				 //
//                  :iSymbolSize	:�o�l�R�[�h�摜�V���{���T�C�Y			 //
// �߂�l			:RESULT_OK												 //
//                  :RESULT_ERROR_SECURE_MEMORY								 //
// ���l				:�摜��ێ�����K�v�������̂ŁA�{�֐��ŏ������s���܂�	 //
// ------------------------------------------------------------------------- //
int CPMCodeMakeHeader::SetPMCodeHeader (char *szImage, UINT iImageSize, int iSymbolSize, int iLayer) {

	int		iBitSize;
	int		iLineSize;
	int		iX, iY;
	int		iDataPos;
	long	lColorCode;
	char	szColor [8];
	char	szColorHex [4];
	char	cTemp;

	m_iLayer		= iLayer;
	m_iSymbolSize	= iSymbolSize;
	m_iVersion		= SYM2VER (m_iSymbolSize);	

// �w���̐ݒ�
	iBitSize	= BIT_COUNT_24 / 8;
	iLineSize	= (int)CalcPitch (BIT_COUNT_24, m_iSymbolSize);

	// �f�[�^�̃Z�b�g
	for (int i = 0; i < LAYER_BIT; i ++) {
		iX = (LAYER_CODE_POS_X * iBitSize);
// 2006/10/12 tsato update ----------------------------------------------
//		iY = 9 + i;
		iY = m_iSymbolSize - (10 + i);
// ----------------------------------------------------------------------
																			// �Y���ʒu���W�̌v�Z
		iDataPos = (iY * iLineSize) + iX;
		memset (szColor, '\0', sizeof (szColor));
		memcpy (szColor, &g_szLayerSetting [m_iLayer - MIN_LAYERSIZE][i * 3], PM_CODE_BIT_SIZE);

// comment out start 2009/11/18 gabu
// Windows�łł́A�ȉ���3�s��R��B�����ւ��āA���܂������Ă��邪�A
// OSS�Łi�J����Mac OS X�j�ł́A�Ȃ���R��B�����]����B
// �ȉ���3�s���R�����g�A�E�g���āAR��B�̓���ւ�����߂�ƁA�f�R�[�h����������B
// �i�ȉ���3�s�ŁAR��B�����ւ���ƁA�f�R�[�h���ɃG���[�ɂȂ�B�j
//		cTemp = szColor [0];
//		szColor [0] = szColor [2];
//		szColor [2] = cTemp;
// comment out end 2009/11/18 gabu
		memcpy (&szImage [iDataPos], szColor, PM_CODE_BIT_SIZE);
	}
// �F�R�[�h�̐ݒ�
#if 0
	for (int i = 0; i < PM_CODE_HEADER_COLOR_CODE; i ++) {
		iX = (COLOR_CODE_POS_X - (i / 4)) * iBitSize;
// 2006/10/12 tsato update ----------------------------------------------
//		iY = 9 + (i % 4);
		iY = m_iSymbolSize - (10 + (i % 4));
// ----------------------------------------------------------------------
		// �Y���ʒu���W�̌v�Z
		iDataPos = (iY * iLineSize) + iX;
		lColorCode = 0;

		 // �F�R�[�h�ݒ�ȏ�̍��ڂ͐ݒ肵�Ȃ�
		if (g_ColorCodeSetting == 0) {										// ��`�t�@�C��
			lColorCode = g_ColorCodeSettingDefinition [i];	
		} else {															// �f�t�H���g�l
			if (m_iLayer == 3) {											// �w���͂R
				if (i < m_iLayer) {
					lColorCode = g_ColorCodeSetting3 [i];
				} else {
					break;													// ����ȏ�͐ݒ肵�Ȃ�
				}
			} else if (m_iLayer > 9) {										// �w���͂P�O�`�Q�S
				if (i < m_iLayer || i < PM_CODE_HEADER_COLOR_CODE) {
					lColorCode = g_ColorCodeSetting10to24 [i];	
				} else {
					break;													// ����ȏ�͐ݒ肵�Ȃ�
				}
			} else {														// �w���͂R�`�X
				if (i < m_iLayer) {
					lColorCode = g_ColorCodeSetting4to9 [i];	
				} else {
					break;													// ����ȏ�͐ݒ肵�Ȃ�
				}
			}
		}
		memset (szColor, '\0', sizeof (szColor));
		sprintf (szColor, "%0.6X", lColorCode);

		cTemp = szColor [0];
		szColor [0] = szColor [4];
		szColor [4] = cTemp;
		cTemp = szColor [1];
		szColor [1] = szColor [5];
		szColor [5] = cTemp;
		
		for (int j = 0; j < PM_CODE_BIT_SIZE; j ++) {
			szColorHex [j] = h2tb1 (&szColor [j * 2]);
		}
		memcpy (&szImage [iDataPos], szColorHex, PM_CODE_BIT_SIZE);
	}
#else 
// 2006/11/28 �F���ʃR�[�h�g��-----------------------------------------------
	for (int i = 0; i < g_ColorCodeTableSize; i ++) {
		if (i < PM_CODE_HEADER_COLOR_CODE) {
			iX = (COLOR_CODE_POS_X - (i / 4)) * iBitSize;
			iY = m_iSymbolSize - (10 + (i % 4));
		} else if (i >= PM_CODE_HEADER_COLOR_CODE && i < PM_CODE_HEADER_COLOR_CODE2){
			iX = (LAYER_CODE_POS_X - ((i - PM_CODE_HEADER_COLOR_CODE) / 4)) * iBitSize;
			iY = m_iSymbolSize - (14 + ((i - PM_CODE_HEADER_COLOR_CODE) % 4));
		} else {
			iX = (LAYER_CODE_POS_X - ((i - PM_CODE_HEADER_COLOR_CODE2) / 4)) * iBitSize;
			iY = m_iSymbolSize - (18 + ((i - PM_CODE_HEADER_COLOR_CODE2) % 4));
		}

		// �Y���ʒu���W�̌v�Z
		iDataPos = (iY * iLineSize) + iX;
		lColorCode = 0;
		lColorCode = g_ColorCodeTable [i];
		memset (szColor, '\0', sizeof (szColor));
		sprintf (szColor, "%0.6X", lColorCode);

		cTemp = szColor [0];
		szColor [0] = szColor [4];
		szColor [4] = cTemp;
		cTemp = szColor [1];
		szColor [1] = szColor [5];
		szColor [5] = cTemp;
		
		for (int j = 0; j < PM_CODE_BIT_SIZE; j ++) {
			szColorHex [j] = h2tb1 (&szColor [j * 2]);
		}
		memcpy (&szImage [iDataPos], szColorHex, PM_CODE_BIT_SIZE);
// --------------------------------------------------------------------------
	}
#endif
	return RESULT_OK;
}
