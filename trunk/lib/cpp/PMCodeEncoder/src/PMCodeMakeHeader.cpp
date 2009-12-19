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

#include "define.h"
#include "PMCodeMakeHeader.h"
#include "global.h"
#include <stdio.h>

#define QR_CODE_BIT_ON		'\x00'
#define QR_CODE_BIT_OFF		'\xFF'

CPMCodeMakeHeader::CPMCodeMakeHeader () {

}

CPMCodeMakeHeader::~CPMCodeMakeHeader () {

}

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

	iBitSize	= BIT_COUNT_24 / 8;
	iLineSize	= (int)CalcPitch (BIT_COUNT_24, m_iSymbolSize);

	for (int i = 0; i < LAYER_BIT; i ++) {
		iX = (LAYER_CODE_POS_X * iBitSize);
		iY = m_iSymbolSize - (10 + i);

		iDataPos = (iY * iLineSize) + iX;
		memset (szColor, '\0', sizeof (szColor));
		memcpy (szColor, &g_szLayerSetting [m_iLayer - MIN_LAYERSIZE][i * 3], PM_CODE_BIT_SIZE);
		memcpy (&szImage [iDataPos], szColor, PM_CODE_BIT_SIZE);
	}
#if 0
	for (int i = 0; i < PM_CODE_HEADER_COLOR_CODE; i ++) {
		iX = (COLOR_CODE_POS_X - (i / 4)) * iBitSize;
		iY = m_iSymbolSize - (10 + (i % 4));

		iDataPos = (iY * iLineSize) + iX;
		lColorCode = 0;

		if (g_ColorCodeSetting == 0) {
			lColorCode = g_ColorCodeSettingDefinition [i];	
		} else {
			if (m_iLayer == 3) {
				if (i < m_iLayer) {
					lColorCode = g_ColorCodeSetting3 [i];
				} else {
					break;
				}
			} else if (m_iLayer > 9) {
				if (i < m_iLayer || i < PM_CODE_HEADER_COLOR_CODE) {
					lColorCode = g_ColorCodeSetting10to24 [i];	
				} else {
					break;
				}
			} else {
				if (i < m_iLayer) {
					lColorCode = g_ColorCodeSetting4to9 [i];	
				} else {
					break;
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
	}
#endif
	return RESULT_OK;
}
