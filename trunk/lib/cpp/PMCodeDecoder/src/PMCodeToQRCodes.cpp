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

#include "PMCodeToQRCodes.h"
#include "Global.h"
#include "define.h"
#include <stdio.h>


CPMCodeToQRCodes::CPMCodeToQRCodes () {

}

CPMCodeToQRCodes::~CPMCodeToQRCodes () {

}

int CPMCodeToQRCodes::PMCodeToQRCode (char *szPMCodeImage, UINT uiPMCodeImageSize, long lColorCode
									, char *szQRCodeImage, UINT uiQRCodeImageSize, int uiSymbolSize) {
	long	lBit;
	int		iBaseImagePos;
	int		iValueImagePos;
	int		iBaseWidthLength;
	int		iValueWidthLength;
	int		iBitSize;
	char	szColor [8];
	char	szTemp	[17];
	char	cTemp;

	iBitSize	= BIT_COUNT_24 / 8;

	iBaseWidthLength = CalcPitch (BIT_COUNT_24, uiSymbolSize);
	iValueWidthLength = CalcPitch (BIT_COUNT_8, uiSymbolSize);

	if (uiPMCodeImageSize < ((unsigned int)iBaseWidthLength * uiSymbolSize)) {
		return RESULT_ERROR_SECURE_MEMORY;	
	}
	if (uiQRCodeImageSize < ((unsigned int)iValueWidthLength * uiSymbolSize)) {
		return RESULT_ERROR_SECURE_MEMORY;	
	}

	memset (szTemp, '\0', sizeof (szTemp));
	sprintf (szTemp, "%0.6X", lColorCode);

	cTemp = szTemp [0];
	szTemp [0] = szTemp [4];
	szTemp [4] = cTemp;
	cTemp = szTemp [1];
	szTemp [1] = szTemp [5];
	szTemp [5] = cTemp;
	lColorCode = AscHexToLong(szTemp, strlen (szTemp));

	for (int i = 0; i < uiSymbolSize; i ++) {
		for (int j = 0; j < uiSymbolSize; j ++) {
			iBaseImagePos = (i * iBaseWidthLength) + (j * iBitSize);

			iValueImagePos = (i * iValueWidthLength) + j;

			memset (szColor, '\0', sizeof (szColor));
			memcpy (szColor, &szPMCodeImage [iBaseImagePos], iBitSize);

			memset (szTemp, '\0', sizeof (szTemp));
			for (int k = 0 ; k < iBitSize; k ++) {
				b1th2 (szColor [k], &szTemp [k * 2]);
			}
			lBit = AscHexToLong (szTemp, iBitSize * 2);

			if (lBit == 0xFFFFFF) {
				szQRCodeImage [iValueImagePos] = BIT_ON;
			} else if (lBit == 0x000000) {
				szQRCodeImage [iValueImagePos] = BIT_OFF;
			} else if ((lBit & lColorCode) == 0x000000){
				szQRCodeImage [iValueImagePos] = BIT_OFF;
			} else {
				szQRCodeImage [iValueImagePos] = BIT_ON;
			}
		}
	}
	return RESULT_OK;
}
