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

#include "QRCodeRestoration.h"
#include "global.h"
#include "define.h"

CQRCodeRestoration::CQRCodeRestoration () {

	m_szQRData = NULL;
}

CQRCodeRestoration::~CQRCodeRestoration () {

	if(m_szQRData != NULL) {
		free (m_szQRData);
		m_szQRData = NULL;
	}
}

int CQRCodeRestoration::SetCodeWordPattern (char * szImage, UINT uiImageSize
											, int iWordSize, int iSymbolSize, int iRSLevel) {

    int iX			= iSymbolSize;
    int iY			= iSymbolSize - 1;
    int iDirectionX = 1;
    int iDirectionY = 1;
	int	iLineLength;

	iLineLength = CalcPitch (BIT_COUNT_8, iSymbolSize);

	if(m_szQRData != NULL) {
		free (m_szQRData);
		m_szQRData	= NULL;
		m_iDataSize = 0;
	}
	m_iDataSize = iWordSize;
	m_szQRData = (char *)malloc (m_iDataSize + 1);
	if (m_szQRData == NULL) {
		return RESULT_ERROR_SECURE_MEMORY;
	}
    memset (m_szQRData, '\0', m_iDataSize + 1);

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
            if ((szImage[(iY * iLineLength) + iX] & 0x01) == 0x01) {
                m_szQRData [i] |= 1 << (7 - j);
            }
        }
    }
	return RESULT_OK;
}

int CQRCodeRestoration::GetCodeData (char * szData, int iDataSize) {
	
	if (m_szQRData != NULL && m_iDataSize != 0) {
		if (iDataSize >= m_iDataSize) {
			memcpy (szData, m_szQRData, m_iDataSize);
		} else {
			memcpy (szData, m_szQRData, iDataSize);
		}
	} else {
		return RESULT_ERROR_DECODE_YET;
	}
	return RESULT_OK;
}
