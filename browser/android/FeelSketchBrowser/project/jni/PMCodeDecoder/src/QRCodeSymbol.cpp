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

#include "QRCodeSymbol.h"
#include "Global.h"
#include "define.h"

CQRCodeSymbol::CQRCodeSymbol () {

}

CQRCodeSymbol::~CQRCodeSymbol () {

}

int CQRCodeSymbol::GetQRCodeSymbol (char * szImage, UINT iImageSize, int iSymbolSize
							  , int *iRSLevel, int *iMaskPattern) {

	int		iModel = 0;
	char	cBit;
	int		i;
	int		iLineLength;

	iLineLength = CalcPitch (BIT_COUNT_8, iSymbolSize);

	for (i = 0; i <= 5; i ++) {
		cBit = szImage [(iLineLength * i) + 8];
		if (cBit == BIT_ON) {
			iModel |= (1 << i);
		}
	}
	if (BIT_ON == szImage [(iLineLength * 7) + 8]) {
		iModel |= (1 << 6);
	}
	if (BIT_ON == szImage [(iLineLength * 8) + 8]) {
		iModel |= (1 << 7);
	}
	if (BIT_ON == szImage [(iLineLength * 8) + 7]) {
		iModel |= (1 << 8);
	}

	for (i = 9; i <= 14; i ++) {
		cBit = szImage [(iLineLength * 8) + (14 - i)];
		if (cBit == BIT_ON) {
			iModel |= (1 << i);
		}
	}

	iModel ^= 0x5412; // 101010000010010b

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

	m_iMaskPattern	= ((iModel >> 10) & 0x07);
	if (m_iMaskPattern < 0 || m_iMaskPattern > 7) {
		return RESULT_ERROR_QR_CODE_HEADER;
	}
	*iMaskPattern = m_iMaskPattern;
	return RESULT_OK;
}

int CQRCodeSymbol::SetQRCodeSymbol (int iSymbolSize, int iRSLevel, int iMaskPattern) {

	int iFormatInfo;

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

	for (int i = 0; i < 5; ++i)	{
		if (iFormatData & (1 << (14 - i))) {
			iFormatData ^= (0x0537 << (4 - i)); // 10100110111b
		}
	}

	iFormatData += iFormatInfo << 10;

	iFormatData ^= 0x5412; // 101010000010010b

	for (int i = 0; i <= 5; ++i) {
		g_byModuleData[8][i] = (iFormatData & (1 << i)) ? '\x30' : '\x20';
	}
	g_byModuleData[8][7] = (iFormatData & (1 << 6)) ? '\x30' : '\x20';
	g_byModuleData[8][8] = (iFormatData & (1 << 7)) ? '\x30' : '\x20';
	g_byModuleData[7][8] = (iFormatData & (1 << 8)) ? '\x30' : '\x20';

	for (int i = 9; i <= 14; ++i) {
		g_byModuleData[14 - i][8] = (iFormatData & (1 << i)) ? '\x30' : '\x20';
	}

	for (int i = 0; i <= 7; ++i) {
		g_byModuleData[iSymbolSize - 1 - i][8] = (iFormatData & (1 << i)) ? '\x30' : '\x20';
	}

	g_byModuleData[8][iSymbolSize - 8] = '\x30';

	for (int i = 8; i <= 14; ++i) {
		g_byModuleData[8][iSymbolSize - 15 + i] = (iFormatData & (1 << i)) ? '\x30' : '\x20';
	}
	return RESULT_OK;
}
