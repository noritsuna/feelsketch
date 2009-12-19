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

#include "QRCodeDisposition.h"
#include "global.h"
#include "define.h"

CQRCodeDisposition::CQRCodeDisposition () {

}

CQRCodeDisposition::~CQRCodeDisposition () {

}

int CQRCodeDisposition::SetCodeWordPattern (char * szData, int iDataSize
											, int iWordSize, int iSymbolSize, int iRSLevel) {

    int iX			= iSymbolSize;
    int iY			= iSymbolSize - 1;
    int iDirectionX = 1;
    int iDirectionY = 1;

	if (iDataSize > iWordSize) {
		return 0;
	}

    for (int i = 0; i < iWordSize; i ++ ){
        for (int j = 0; j < 8; j ++) {
            do {
				iX += iDirectionX;
				iDirectionX *= -1;

				if (iDirectionX < 0) {
					iY += iDirectionY;

					if (iY < 0 || iY == iSymbolSize) {
						if (iY < 0) {
							iY = 0;
						} else {
							iY = iSymbolSize - 1;
						}
						iDirectionY *= -1;

						iX -= 2;

						if (iX == 6)
							iX -= 1;
					}
				}
            } while (g_byModuleData[iX][iY] & 0x20);
			g_byModuleData[iX][iY] = (szData[i] & (1 << (7 - j))) ? '\x02' : '\x00';
        }
    }

	return RESULT_OK;
}

