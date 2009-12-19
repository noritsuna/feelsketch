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

#include "QRCodeToPMCode.h"
#include "global.h"
#include "define.h"
#include <stdio.h>

CQRCodeToPMCode::CQRCodeToPMCode () {

}

CQRCodeToPMCode::~CQRCodeToPMCode () {

}

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

	for (int i = 0; i < uiSymbolSize; i ++) {
		for (int j = 0; j < uiSymbolSize; j ++) {
			iBaseImagePos = (i * iBaseWidthLength) + j;

			iValueImagePos = (i * iValueWidthLength) + (j * iBitSize);

			if ((szQRCodeImage [iBaseImagePos] & BIT_ON) == BIT_ON) {
				szPMCodeImage [iValueImagePos] = szPMCodeImage [iValueImagePos] | h2tb1 (&szColor [0]);
				szPMCodeImage [iValueImagePos + 1] = szPMCodeImage [iValueImagePos + 1] | h2tb1 (&szColor [2]);
				szPMCodeImage [iValueImagePos + 2] = szPMCodeImage [iValueImagePos + 2] | h2tb1 (&szColor [4]);
			}
		}
	}
	return RESULT_OK;
}
