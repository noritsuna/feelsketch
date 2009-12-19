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

#include "QRCodeMask.h"
#include "define.h"
#include "global.h"

CQRCodeMask::CQRCodeMask () {

}

CQRCodeMask::~CQRCodeMask () {

}

void CQRCodeMask::GetMaskQRCode (char * szImage, UINT iImageSize, int iMaskPattern, int iVersion) {
	
	int		iLineLength;
	int		iSymbolSize = VER2SYM (iVersion);

	iLineLength = CalcPitch (BIT_COUNT_8, iSymbolSize);
	
	for (int i = 0; i < iSymbolSize; i ++) {
		for (int j = 0; j < iSymbolSize; j ++) {
			if (! (g_byModuleData[j][i] & 0x20)) {
				BOOL bMask;

				switch (iMaskPattern) {
				case 0:
					bMask = ((i + j) % 2 == 0);
					break;

				case 1:
					bMask = (i % 2 == 0);
					break;

				case 2:
					bMask = (j % 3 == 0);
					break;

				case 3:
					bMask = ((i + j) % 3 == 0);
					break;

				case 4:
					bMask = (((i / 2) + (j / 3)) % 2 == 0);
					break;

				case 5:
					bMask = (((i * j) % 2) + ((i * j) % 3) == 0);
					break;

				case 6:
					bMask = ((((i * j) % 2) + ((i * j) % 3)) % 2 == 0);
					break;

				default: // case 7:
					bMask = ((((i * j) % 3) + ((i + j) % 2)) % 2 == 0);
					break;
				}
				szImage[(i * iLineLength) + j] = (char)(szImage[(i * iLineLength) + j] ^ bMask);
			}
		}
	}
}

void CQRCodeMask::SetMaskQRCode (int iMaskPattern, int iVersion) {
	
	int	iSymbolSize = VER2SYM (iVersion);
	
	for (int i = 0; i < iSymbolSize; i ++) {
		for (int j = 0; j < iSymbolSize; j ++) {
			if (! (g_byModuleData[j][i] & 0x20)) {
				BOOL bMask;

				switch (iMaskPattern) {
				case 0:
					bMask = ((i + j) % 2 == 0);
					break;

				case 1:
					bMask = (i % 2 == 0);
					break;

				case 2:
					bMask = (j % 3 == 0);
					break;

				case 3:
					bMask = ((i + j) % 3 == 0);
					break;

				case 4:
					bMask = (((i / 2) + (j / 3)) % 2 == 0);
					break;

				case 5:
					bMask = (((i * j) % 2) + ((i * j) % 3) == 0);
					break;

				case 6:
					bMask = ((((i * j) % 2) + ((i * j) % 3)) % 2 == 0);
					break;

				default: // case 7:
					bMask = ((((i * j) % 3) + ((i + j) % 2)) % 2 == 0);
					break;
				}
				g_byModuleData[j][i] = (BYTE)((g_byModuleData[j][i] & 0xfe) | (((g_byModuleData[j][i] & 0x02) > 1) ^ bMask));
			}
		}
	}
}
