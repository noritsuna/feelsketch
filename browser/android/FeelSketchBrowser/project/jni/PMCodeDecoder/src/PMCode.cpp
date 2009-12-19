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

#include <stdio.h>
#include "global.h"
#include "PMCodeReader.h"
#include "data.h"

#define BLACK_COLOR_CODE		"000000"


int decodePMCode () {

	int		iRet;
	int		iLayer;
	int		iRSLevel;
	int		iVersion;
	int		iModuleSize;
	int		iOldCount;
	unsigned int uiSize;
	char	szReserve [16];
	char	szExt [8];
	char	szSize [8];
	char	szSizeBuf [16];

	int     m_iDataSize;
	char*   m_szDataBuf = NULL;

	char *m_szPMCodeImage = (char*)imageData;
	UINT m_uiPMCodeImageSize = sizeof(imageData);
	int m_iHeight = 29;
	int m_iWidth = 29;


	CPMCodeReader m_cPMCodeReader;

	m_cPMCodeReader.SetBaseImage(m_szPMCodeImage, m_uiPMCodeImageSize, m_iHeight, m_iWidth);


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

	iRet = m_cPMCodeReader.GetDecodeData (m_szDataBuf, m_iDataSize);
	if (iRet != RESULT_OK){
		return iRet;
	}

	memset (szExt, '\0', sizeof (szExt));
	memcpy (szExt, m_szDataBuf, EXT_SIZE);
	memset (szSizeBuf, '\0', sizeof (szSizeBuf));
	memcpy (szSize, &m_szDataBuf [EXT_SIZE], SIZE_SIZE);
	memset (szReserve, '\0', sizeof (szReserve));
	memcpy (szReserve, &m_szDataBuf [EXT_SIZE + SIZE_SIZE], RESERVE_SIZE);

	for (int i = 0; i < SIZE_SIZE; i ++ ){
		b1th2(szSize[i], &szSizeBuf [i * 2] );
	}
	uiSize = AscHexToLong(szSizeBuf, strlen (szSizeBuf));

	printf("Load Data = %s\n", &m_szDataBuf[HEADER_SIZE]);

	free(m_szDataBuf);
	if(&m_cPMCodeReader != NULL)
		delete &m_cPMCodeReader;
	return iRet;

}



int main() {
	int ret = decodePMCode();
	printf("ret = %i\n", ret);
	return 0;
}

