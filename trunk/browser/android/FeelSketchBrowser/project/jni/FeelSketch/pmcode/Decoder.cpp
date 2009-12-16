/**
***                          "Feel Sketch" browser.
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

#include "Decoder.h"
#include <stdio.h>

namespace pmcode {
Decoder::Decoder() {
}

Decoder::~Decoder() {
}

int Decoder::decode(unsigned char* imageData, int dataSize, int dimension, char* result_data, char* result_ext, char* result_reserve, int* result_data_size, int *baseColors, int layer) {

	int		iRet;
	int		iLayer;
	int		iRSLevel;
	int		iVersion;
	int		iModuleSize;
	int		iOldCount;
	int   	uiSize;
	char	szReserve [16];
	char	szExt [8];
	char	szSize [8];
	char	szSizeBuf [16];

	int     m_iDataSize;
	char*   m_szDataBuf = NULL;

	// image parameters
	char *m_szPMCodeImage = (char*)imageData;
	UINT m_uiPMCodeImageSize = dataSize;
	int m_iHeight = dimension;
	int m_iWidth = dimension;

	CPMCodeReader m_cPMCodeReader;
	m_cPMCodeReader.SetBaseImage(m_szPMCodeImage, m_uiPMCodeImageSize, m_iHeight, m_iWidth);
	m_cPMCodeReader.SetBaseColor(baseColors, layer);
	m_iDataSize = m_cPMCodeReader.DecodePMCodeImage ();
	if (m_iDataSize < 0) {
		return m_iDataSize;
	}
	if (m_szDataBuf != NULL){
		free (m_szDataBuf);
		m_szDataBuf = NULL;
	}

	m_szDataBuf = (char *)malloc (m_iDataSize + 1);
	if (m_szDataBuf == NULL){
		return RESULT_ERROR_SECURE_MEMORY;
	}
	memset (m_szDataBuf, '\0', m_iDataSize + 1);

	iRet = m_cPMCodeReader.GetDecodeData (m_szDataBuf, m_iDataSize);			// 復号データの取得
	if (iRet != RESULT_OK){
		return iRet;
	}

	// get result data
	// extension
	memset (szExt, '\0', sizeof (szExt));
	memcpy (szExt, m_szDataBuf, EXT_SIZE);
	memcpy (result_ext, m_szDataBuf, EXT_SIZE);
	// size
	memset (szSizeBuf, '\0', sizeof (szSizeBuf));
	memcpy (szSize, &m_szDataBuf [EXT_SIZE], SIZE_SIZE);
	// reserve
	memset (szReserve, '\0', sizeof (szReserve));
	memcpy (szReserve, &m_szDataBuf [EXT_SIZE + SIZE_SIZE], RESERVE_SIZE);
	memcpy (result_reserve, &m_szDataBuf [EXT_SIZE + SIZE_SIZE], RESERVE_SIZE);

	for (int i = 0; i < SIZE_SIZE; i ++ ){
		b1th2(szSize[i], &szSizeBuf [i * 2] );
	}
	uiSize = AscHexToLong(szSizeBuf, strlen (szSizeBuf));

	memcpy (result_data, &m_szDataBuf [HEADER_SIZE], uiSize);
	*result_data_size = uiSize;

	free(m_szDataBuf);
	return iRet;
}
}
