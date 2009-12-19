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
#include "Global.h"
#include "PMCodeReader.h"

CPMCodeReader::CPMCodeReader () {

	m_szPMCodeImage = NULL;
}

CPMCodeReader::~CPMCodeReader () {

}

UINT CPMCodeReader::DecodePMCodeImage () {

	int		DataSize;

	DataSize = DecodePMCode ();
	return DataSize;
}

int CPMCodeReader::GetQRCodeData (char ** szData, UINT *uiImageSize, int *iWidth, int *iHeight, int iLayer) {

	int		iRet;

	iRet = GetQRCodeImage (szData, uiImageSize, iWidth, iHeight, iLayer);
	if(RESULT_OK != iRet){
		return iRet;
	}
	return 0;
}

int	CPMCodeReader::GetDecodeData (char *szData, int szDataSize) {

	int iRet;
	iRet = GetPMCodeDecodeData (szData, szDataSize);
	return iRet;
}

