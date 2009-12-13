#include "QRCodeSymbol.h"
#include "Global.h"
#include "define.h"
//#include "CtrlBMP.h"

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�R���X�g���N�^											 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
CQRCodeSymbol::CQRCodeSymbol () {

}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�f�X�g���N�^											 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
CQRCodeSymbol::~CQRCodeSymbol () {

}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�V���{���i�^���j�̎擾									 //
// ����				:szImage			:�p�q�摜�f�[�^						 //
//					:iImageSize			:�摜�T�C�Y							 //
//					:iSymbolSize		:�V���{���T�C�Y						 //
//					:iRSLevel			:���������x��						 //
//					:iMaskPattern		:�}�X�N�p�^�[��						 //
// �߂�l			:RESULT_OK												 //
//					:RESULT_ERROR_QR_CODE_HEADER							 //
// ���l				:�摜��ێ�����K�v�������ׁA�{�֐��ɂĉ�͂��s���܂�	 //
// ------------------------------------------------------------------------- //
int CQRCodeSymbol::GetQRCodeSymbol (char * szImage, UINT iImageSize, int iSymbolSize
							  , int *iRSLevel, int *iMaskPattern) {

	int		iModel = 0;
	char	cBit;
	int		i;
	int		iLineLength;

	iLineLength = CalcPitch (BIT_COUNT_8, iSymbolSize);

	for (i = 0; i <= 5; i ++) {
// 2006/10/11 tsato update ----------------------------------------------
		cBit = szImage [(iLineLength * i) + 8];
//		cBit = szImage [iLineLength * (iSymbolSize - (i + 1)) + 8];
// ----------------------------------------------------------------------
		if (cBit == BIT_ON) {												// �r�b�g���n�m
			iModel |= (1 << i);
		}
	}
// 2006/10/11 tsato update ----------------------------------------------
	if (BIT_ON == szImage [(iLineLength * 7) + 8]) {			// �r�b�g���n�m
//	if (BIT_ON == szImage [iLineLength * (iSymbolSize - 8) + 8]) {			// �r�b�g���n�m
// ----------------------------------------------------------------------
		iModel |= (1 << 6);
	}
// 2006/10/11 tsato update ----------------------------------------------
	if (BIT_ON == szImage [(iLineLength * 8) + 8]) {			// �r�b�g���n�m
//	if (BIT_ON == szImage [iLineLength * (iSymbolSize - 9) + 8]) {			// �r�b�g���n�m
// ----------------------------------------------------------------------
		iModel |= (1 << 7);
	}
// 2006/10/11 tsato update ----------------------------------------------
	if (BIT_ON == szImage [(iLineLength * 8) + 7]) {			// �r�b�g���n�m
// ----------------------------------------------------------------------
		iModel |= (1 << 8);
	}

	for (i = 9; i <= 14; i ++) {
// 2006/10/11 tsato update ----------------------------------------------
		cBit = szImage [(iLineLength * 8) + (14 - i)];
// ----------------------------------------------------------------------
		if (cBit == BIT_ON) {												// �r�b�g���n�m
			iModel |= (1 << i);
		}
	}

	// �}�X�L���O
	iModel ^= 0x5412; // 101010000010010b

	// �������i���ݖ��쐬�j


	// ���������x���̎擾
	m_iRSLevel		= iModel >> 13;
	switch (m_iRSLevel) {
	case 1:
		*iRSLevel = QR_LEVEL_L;
		break;
	case 0:
		*iRSLevel = QR_LEVEL_M;
		break;
	case 3:
		*iRSLevel = QR_LEVEL_Q;
		break;
	case 2:
		*iRSLevel = QR_LEVEL_H;
		break;

	default: // case QR_LEVEL_H:
		return RESULT_ERROR_QR_CODE_HEADER;

	}

/*
	if (m_iRSLevel != QR_LEVEL_L && m_iRSLevel != QR_LEVEL_M 
		&& m_iRSLevel != QR_LEVEL_Q && m_iRSLevel != QR_LEVEL_H){		// ���������x���̃`�F�b�N�i�O�`�R�j�ȊO�̓G���[
		return RESULT_ERROR_QR_CODE_HEADER;
	}
	*iRSLevel = m_iRSLevel;
*/
	// �}�X�N�p�^�[���̎擾
	m_iMaskPattern	= ((iModel >> 10) & 0x07);
	if (m_iMaskPattern < 0 || m_iMaskPattern > 7) {						// �}�X�N�p�^�[���̃`�F�b�N�i�O�`�V�j�ȊO�̓G���[
		return RESULT_ERROR_QR_CODE_HEADER;
	}
	*iMaskPattern = m_iMaskPattern;
	return RESULT_OK;
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�V���{���i�^���j�̐ݒ�									 //
// ����				:iSymbolSize		:�V���{���T�C�Y						 //
//					:iRSLevel			:���������x��						 //
//					:iMaskPattern		:�}�X�N�p�^�[��						 //
// �߂�l			:RESULT_OK												 //
//					:RESULT_ERROR_QR_CODE_HEADER							 //
// ���l				:�摜��ێ�����K�v�������ׁA�{�֐��ɂĉ�͂��s���܂�	 //
// ------------------------------------------------------------------------- //
int CQRCodeSymbol::SetQRCodeSymbol (int iSymbolSize, int iRSLevel, int iMaskPattern) {

	int iFormatInfo;

// �^���̐ݒ�
	switch (iRSLevel) {
	case QR_LEVEL_L:
		iFormatInfo = 0x08; // 01nnnb
		break;

	case QR_LEVEL_M:
		iFormatInfo = 0x00; // 00nnnb
		break;

	case QR_LEVEL_Q:
		iFormatInfo = 0x18; // 11nnnb
		break;

	case QR_LEVEL_H:
		iFormatInfo = 0x10; // 10nnnb
		break;

	default: // case QR_LEVEL_H:
		;
	}

	iFormatInfo += iMaskPattern;

	int iFormatData = iFormatInfo << 10;

	// ��]�r�b�g�Z�o
	for (int i = 0; i < 5; ++i)	{
		if (iFormatData & (1 << (14 - i))) {
			iFormatData ^= (0x0537 << (4 - i)); // 10100110111b
		}
	}

	iFormatData += iFormatInfo << 10;

	// �}�X�L���O
	iFormatData ^= 0x5412; // 101010000010010b

	// ����ʒu���o�p�^�[������z�u
	for (int i = 0; i <= 5; ++i) {
		g_byModuleData[8][i] = (iFormatData & (1 << i)) ? '\x30' : '\x20';
	}
	g_byModuleData[8][7] = (iFormatData & (1 << 6)) ? '\x30' : '\x20';
	g_byModuleData[8][8] = (iFormatData & (1 << 7)) ? '\x30' : '\x20';
	g_byModuleData[7][8] = (iFormatData & (1 << 8)) ? '\x30' : '\x20';

	for (int i = 9; i <= 14; ++i) {
		g_byModuleData[14 - i][8] = (iFormatData & (1 << i)) ? '\x30' : '\x20';
	}

	// �E��ʒu���o�p�^�[�����z�u
	for (int i = 0; i <= 7; ++i) {
		g_byModuleData[iSymbolSize - 1 - i][8] = (iFormatData & (1 << i)) ? '\x30' : '\x20';
	}

	// �����ʒu���o�p�^�[���E�z�u
	g_byModuleData[8][iSymbolSize - 8] = '\x30'; // �Œ�Ã��W���[��

	for (int i = 8; i <= 14; ++i) {
		g_byModuleData[8][iSymbolSize - 15 + i] = (iFormatData & (1 << i)) ? '\x30' : '\x20';
	}
	return RESULT_OK;
}
