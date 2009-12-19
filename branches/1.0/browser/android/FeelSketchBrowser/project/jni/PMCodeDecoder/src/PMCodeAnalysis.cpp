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
#include "PMCodeAnalysis.h"
#include "Global.h"


CPMCodeAnalysis::CPMCodeAnalysis () {

	m_iLayerCount = 0;
}

CPMCodeAnalysis::~CPMCodeAnalysis () {

}

int CPMCodeAnalysis::GetLayerCount (char *szImage, UINT iImageSize, int iSymbolSize) {

	char	szbuf	[32];
	int		iX, iY;
	int		iDataPos;
	int		iBitSize;
	int		iLineSize;
	char	cTemp [2];

	iBitSize	= BIT_COUNT_24 / 8;
	iLineSize	= (int)CalcPitch (BIT_COUNT_24, iSymbolSize);

	memset (szbuf, '\0', sizeof (szbuf));
	for (int i = 0; i < LAYER_BIT; i ++) {
		iX = (LAYER_CODE_POS_X * iBitSize);
		iY = (9 + i);
		iDataPos = (iY * iLineSize) + iX;
		memcpy (&szbuf [i * PM_CODE_BIT_SIZE], &szImage [iDataPos], PM_CODE_BIT_SIZE);
	}
	m_iLayerCount = 0;

	for (int i = 0; i < LAYER_BIT; i ++) {
		cTemp [0] = szbuf [i * iBitSize];
		szbuf [i * iBitSize] = szbuf [(i * iBitSize) + 2];
		szbuf [(i * iBitSize) + 2] = cTemp [0];
	}

	for (int i = 0; i <= PM_CODE_HEADER_LAYER_CODE; i ++) {
		if (((BYTE)szbuf [i]) < (0xFF / 2)) {
			szbuf [i] = 0x00;
		} else {
			szbuf [i] = '\xFF';
		}
	}

	for (int i = 0; i <= PM_CODE_HEADER_LAYER_CODE; i ++) {
		if (memcmp (szbuf, &g_szLayerSetting [i][0], LAYER_CODE_BUF_SIZE) == 0) {
			m_iLayerCount = i + MIN_LAYERSIZE;
			break;
		}
	}
	return m_iLayerCount;
}

int	CPMCodeAnalysis::GetColorCode (long * lColorCode, char *szImage, UINT iImageSize, int iSymbolSize, int iVersion) {
	
	char	szbuf	[32];
	char	szTemp	[32];
	int		iX, iY;
	int		iDataPos;
	int		iBitSize;
	int		iLineSize;
	char	cTemp [2];
	long	lWorkColor [COLORTABLE_SIZE];

	iBitSize	= BIT_COUNT_24 / 8;
	iLineSize	= (int)CalcPitch (BIT_COUNT_24, iSymbolSize);
	if (iVersion < 3) {
		g_ColorCodeTableSize = PM_CODE_HEADER_COLOR_CODE;
	} else {
		g_ColorCodeTableSize = COLORTABLE_SIZE;
	}

	for (int i = 0; i < g_ColorCodeTableSize; i ++) {
		if (i < PM_CODE_HEADER_COLOR_CODE) {
			iX = (COLOR_CODE_POS_X - (i / 4)) * iBitSize;
			iY = (9 + (i % 4));
		} else if (i >= PM_CODE_HEADER_COLOR_CODE && i < PM_CODE_HEADER_COLOR_CODE2){
			iX = (LAYER_CODE_POS_X - ((i - PM_CODE_HEADER_COLOR_CODE) / 4)) * iBitSize;
			iY = (13 + ((i - PM_CODE_HEADER_COLOR_CODE) % 4));
		} else {
			iX = (LAYER_CODE_POS_X - ((i - PM_CODE_HEADER_COLOR_CODE2) / 4)) * iBitSize;
			iY = (17 + ((i - PM_CODE_HEADER_COLOR_CODE2) % 4));
		}
		iDataPos = (iY * iLineSize) + iX;
		memcpy (szbuf, &szImage [iDataPos], PM_CODE_BIT_SIZE);
		memset (szTemp, '\0', sizeof (szTemp));
		cTemp [0]= szbuf [0];
		szbuf [0] = szbuf [2];
		szbuf [2] = cTemp [0];

		for (int j = 0 ; j < PM_CODE_BIT_SIZE; j ++) {
			b1th2 (szbuf [j], &szTemp [j * 2]);
		}
		lWorkColor [i] = AscHexToLong (szTemp, PM_CODE_BIT_SIZE * 2);
	}

	for (int i = 0; i < COLORTABLE_SIZE; i ++){
		lColorCode [i] = lWorkColor [i];
	}
	return RESULT_OK;
}
