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
#include "QRCodeEncode.h"
#include "PMCodeEncode.h"
#include "ReedSolomon.h"

#include <stdio.h>

#define MARGIN	4


CPMCodeEncode::CPMCodeEncode () {

	m_szEncodeData		= NULL;
	m_uiDataSize		= 0;
	m_szPMCodeImage		= NULL;
	m_uiPMCodeImageSize	= 0;
	for (int i =0; i < MAX_LAYERSIZE; i ++){
		m_szQRCodeImage	[i]	= NULL;
		m_uiQRCodeImageSize	[i] = 0;
	}
}

CPMCodeEncode::~CPMCodeEncode () {

	if (m_szEncodeData != NULL) {
		free (m_szEncodeData);
		m_szEncodeData	= NULL;
		m_uiDataSize	= 0;
	}

	if (m_szPMCodeImage != NULL) {
		free (m_szPMCodeImage);
		m_szPMCodeImage	= NULL;
		m_uiDataSize	= 0;
	}

	for (int i = 0; i < MAX_LAYERSIZE; i ++){
		if (m_szQRCodeImage != NULL) {
			free (m_szQRCodeImage [i]);
			m_szQRCodeImage	[i]		= NULL;
			m_uiQRCodeImageSize	[i] = 0;
		}
	}
}

int CPMCodeEncode::SetPMCodeData (char *szData, UINT uiDataSize, int iLayer
								  , int iVersion, int iRSLevel, BOOL bMaskAuto, int iModuleSize) {

	if (m_szEncodeData != NULL) {
		free (m_szEncodeData);
		m_szEncodeData	= NULL;
		m_uiDataSize	= 0;
	}
	m_szEncodeData = (char *)realloc (m_szEncodeData
		, QR_VersonInfo[iVersion].ncDataCodeWord [iRSLevel] * iLayer);
	if (m_szEncodeData == NULL) {
		return RESULT_ERROR_SECURE_MEMORY;
	} 
	memset (m_szEncodeData, '\0'
		, QR_VersonInfo[iVersion].ncDataCodeWord [iRSLevel] * iLayer);
	memcpy (m_szEncodeData, szData, uiDataSize);
	m_uiDataSize = uiDataSize;

	m_iLayer		= iLayer;
	m_iVersion		= iVersion;
	m_iRSLevel		= iRSLevel;
	m_bMaskAuto		= bMaskAuto;
	m_iWidth		= VER2SYM (iVersion);
	m_iHeight		= VER2SYM (iVersion);
	m_iModuleSize	= iModuleSize;
	return RESULT_OK;

}

int CPMCodeEncode::SetPMCodeData (char *data, UINT data_size, int layer
								  , int version, int error_correction, BOOL use_mask, int module_size
								  , char *extension, char *note) {
	char 	*szData;
	char	szHeader [32];
	memset (szHeader, '\0', sizeof (szHeader));
	char	szSize [16];
	memset (szSize, '\0', sizeof (szSize));
	char	szHexSize [4];

	if (extension != NULL) {
		if (strlen (extension) < EXT_SIZE) {
			memcpy (szHeader, extension, strlen (extension));
		} else {
			memcpy (szHeader, extension, EXT_SIZE);
		}
	}
	sprintf (szSize, "%0.8X", data_size);
	for (int i = 0; i < 4; i ++) {
		szHexSize [i] = h2tb1 (&szSize [i * 2]);
	}
	memcpy (&szHeader[EXT_SIZE], szHexSize, SIZE_SIZE);
	if (note != NULL) {
		if (strlen (note) < RESERVE_SIZE) {
			memcpy (&szHeader[EXT_SIZE + SIZE_SIZE], note, strlen (note));
		} else {
			memcpy (&szHeader[EXT_SIZE + SIZE_SIZE], note, RESERVE_SIZE);
		}
	}

	szData = (char *)malloc (data_size + EXT_SIZE + SIZE_SIZE + RESERVE_SIZE + 1);
	memset (szData, '\0', data_size + 1);
	memcpy (szData, szHeader, EXT_SIZE + SIZE_SIZE + RESERVE_SIZE);
	memcpy (&szData[EXT_SIZE + SIZE_SIZE + RESERVE_SIZE], data, data_size + 1);

	return SetPMCodeData(szData,
						data_size + EXT_SIZE + SIZE_SIZE + RESERVE_SIZE,
						layer,
						version,
						error_correction,
						use_mask,
						module_size);
}

BOOL CPMCodeEncode::CheckDataSizeToPMCode (UINT uiDataSize, int iLayer, int iVersion, int iRSLevel) {

	UINT	uiLayerDataSize;

	uiLayerDataSize = QR_VersonInfo[iVersion].ncDataCodeWord [iRSLevel] * iLayer;

	if (uiLayerDataSize >= uiDataSize){
		return TRUE;
	} else {
		return FALSE;
	}
}

int CPMCodeEncode::EncodePMCode () {

	CQRCodeEncode	cQRCodeEncode;
	CReedSolomon	cReedSolomon;
	BOOL	bRet;
	int		iRet;
	int		iWordSize;
	char	*szOneLayerData;
	char	*szTemp;
	int		iOneLayerDataMaxSize;
	int		iOneLayerDataSize;
	int		iQRCodeSize;
	long	lColorCode;
	int		iLength;
	int		iMaskPattern;
	int		iPos;
	int		iDataSize;
	int		iBlockSize;
	int		iCorrectPos = 0;
	int		iDataPos = 0;
	int		iCorrectDataSize;
	int		iRSBlock1;
	int		iRSBlock2;
	char	szWhite [4] = {'\xFF', '\xFF', '\xFF', '\x00'};
	char	szBlack [4] = {'\x00', '\x00', '\x00', '\x00'};
	BYTE	szCorrectBuf [256];
	BYTE	szWork [256];
	

	bRet = CheckDataSizeToPMCode (m_uiDataSize, m_iLayer, m_iVersion, m_iRSLevel);
	if (bRet != TRUE) {
		return RESULT_ERROR_SIZE_TO_SETTING;
	}
	m_iSymbolSize  = VER2SYM(m_iVersion);

	for (int i = 0; i < MAX_LAYERSIZE; i ++){
		if (m_szQRCodeImage != NULL) {
			free (m_szQRCodeImage [i]);
			m_szQRCodeImage	[i]	= NULL;
			m_uiQRCodeImageSize	[i] = 0;
		}
	}

	iWordSize = QR_VersonInfo[m_iVersion].ncAllCodeWord;
	szOneLayerData = (char *)malloc(iWordSize + 1);
	if (szOneLayerData == NULL){
		return RESULT_ERROR_SECURE_MEMORY;
	}

	iOneLayerDataMaxSize = QR_VersonInfo[m_iVersion].ncDataCodeWord [m_iRSLevel];

	for (int i = 0; i < m_iLayer; i ++) {
		memset (szOneLayerData, '\0', iWordSize + 1);

		if ((iOneLayerDataMaxSize * (i + 1)) < (int)m_uiDataSize) {
			iOneLayerDataSize = iOneLayerDataMaxSize;
		} else {
			iOneLayerDataSize = m_uiDataSize - (iOneLayerDataMaxSize * i);
		}
		if (iOneLayerDataSize < 0) {
			iOneLayerDataSize = 0;
		}
		iDataPos = 0;
		iDataSize = iOneLayerDataSize;
		iRSBlock1 = QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncRSBlock;
		iRSBlock2 = QR_VersonInfo[m_iVersion].RS_BlockInfo2 [m_iRSLevel].ncRSBlock;

		for (int j = 0; j < (iRSBlock1 + iRSBlock2); j ++) {
			if (iRSBlock1 > j){
				iBlockSize = QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncDataCodeWord;
				cReedSolomon.SetCorrectCodeSize (QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncAllCodeWord	- iBlockSize);
			} else {
				iBlockSize = QR_VersonInfo[m_iVersion].RS_BlockInfo2 [m_iRSLevel].ncDataCodeWord;
				cReedSolomon.SetCorrectCodeSize (QR_VersonInfo[m_iVersion].RS_BlockInfo2 [m_iRSLevel].ncAllCodeWord - iBlockSize);
			}
			memset (szWork, NULL, sizeof (szWork));
			memset (szCorrectBuf, NULL, sizeof (szCorrectBuf));

			memcpy (szWork, &m_szEncodeData [iCorrectPos], iBlockSize);
			cReedSolomon.Encode (szWork, iBlockSize, szCorrectBuf, &iCorrectDataSize);

			memcpy (&szOneLayerData [iDataPos], szCorrectBuf, iCorrectDataSize);
			iCorrectPos += iBlockSize;
			iDataPos += iCorrectDataSize;
			iDataSize -= iBlockSize;
		}

		if (m_bMaskAuto == TRUE){
			iMaskPattern = -1;
		} else {
			iMaskPattern = g_MaskSetting [i];
		}
		iRet = cQRCodeEncode.SetQRCodeData (szOneLayerData, iDataPos
										, m_iVersion, m_iRSLevel, iMaskPattern);
		if (iRet != RESULT_OK) {
			free (szOneLayerData);
			return bRet;
		}
		iQRCodeSize = cQRCodeEncode.EncodeQRCode ();
		if (iQRCodeSize == 0) {
			free (szOneLayerData);
			return RESULT_ERROR_DECODE_FAILURE;
		}
		m_uiQRCodeImageSize	[i] = iQRCodeSize;
		m_szQRCodeImage [i] = (char *)malloc (m_uiQRCodeImageSize [i] + 1);
		if (m_szQRCodeImage [i] == NULL){
			free (szOneLayerData);
			return RESULT_ERROR_SECURE_MEMORY;
		}
		memset (m_szQRCodeImage	[i], '\0', m_uiQRCodeImageSize [i] + 1);

		iRet = cQRCodeEncode.GetQRCodeImage (m_szQRCodeImage [i], m_uiQRCodeImageSize [i]);
		if (iRet != RESULT_OK) {
			free (szOneLayerData);
			return bRet;
		}
	}
	free (szOneLayerData);

	iLength = CalcPitch (BIT_COUNT_24, m_iSymbolSize);
	m_uiPMCodeImageSize = iLength * m_iSymbolSize;
	if (m_szPMCodeImage != NULL){
		free (m_szPMCodeImage);
		m_szPMCodeImage = NULL;
	}
	m_szPMCodeImage = (char *)malloc (m_uiPMCodeImageSize + 1);
	if (m_szPMCodeImage == NULL) {
		return RESULT_ERROR_SECURE_MEMORY;
	}
	memset (m_szPMCodeImage, '\0', m_uiPMCodeImageSize + 1);				

	for (int i = 0; i < m_iLayer; i ++) {
		if (g_ColorCodeSetting == 0) {
			lColorCode = g_ColorCodeSettingDefinition [i];	
		} else {
			if (m_iLayer == 3) {
				lColorCode = g_ColorCodeSetting3 [i];	
			} else if (m_iLayer >9) {
				lColorCode = g_ColorCodeSetting10to24 [i];	
			} else {
				lColorCode = g_ColorCodeSetting4to9 [i];	
			}
		}
		iRet = QRCodeToPMCode (m_szQRCodeImage [i], m_uiQRCodeImageSize	[i], lColorCode
						, m_szPMCodeImage, m_uiPMCodeImageSize, m_iSymbolSize);
	}

	for (int i = 0; i < 9; i ++) {
		g_byModuleData[i][8] = g_byModuleData[8][i] = '\x00';
	}

	for (int i = 0; i < 8; i ++) {
		g_byModuleData[m_iSymbolSize - 8 + i][8] = g_byModuleData[8][m_iSymbolSize - 8 + i] = '\x00';
	}

	for (int i = 0; i < m_iSymbolSize; i ++) {
		for (int j = 0; j < m_iSymbolSize; j ++) {
			if (g_byModuleData[j][m_iSymbolSize - (i + 1)] & 0x20) {
				iPos = (i * iLength) + (j * PM_CODE_BIT_SIZE);
				if (memcmp (&m_szPMCodeImage [iPos], szBlack, PM_CODE_BIT_SIZE) != 0) {
					memcpy (&m_szPMCodeImage [iPos], szWhite, PM_CODE_BIT_SIZE);
				}
			}
		}
	}
	
	iRet = SetPMCodeHeader (m_szPMCodeImage, m_uiPMCodeImageSize, m_iSymbolSize, m_iLayer);
	if (iRet != RESULT_OK) {
		return bRet;
	}

	szTemp = (char *)malloc (m_uiPMCodeImageSize);
	if (szTemp == NULL) {
		return RESULT_ERROR_SECURE_MEMORY;
	}
	memset (szTemp, '\0', sizeof (szTemp));
	memcpy (szTemp, m_szPMCodeImage, m_uiPMCodeImageSize);
	m_iWidth	+= MARGIN;
	m_iHeight	+= MARGIN;
	m_uiPMCodeImageSize = CalcPitch (BIT_COUNT_24, m_iWidth) * m_iHeight;
	m_szPMCodeImage = (char *)realloc (m_szPMCodeImage, m_uiPMCodeImageSize);
	memset (m_szPMCodeImage, '\0', m_uiPMCodeImageSize);
	for (int i = 0; i < (m_iHeight - MARGIN); i ++) {
		iPos = (i + (MARGIN / 2)) * CalcPitch (BIT_COUNT_24, m_iWidth);
		iPos += ((MARGIN / 2) * PM_CODE_BIT_SIZE);
		memcpy (&m_szPMCodeImage [iPos], &szTemp [i * iLength], iLength);
	}
	free (szTemp);

	ImageUtil::UpScale ((unsigned char **)&m_szPMCodeImage, (m_iWidth * m_iModuleSize), (m_iHeight * m_iModuleSize), m_iWidth, m_iHeight);
	ImageUtil::UpDownReplace((unsigned char *)m_szPMCodeImage, (m_iWidth * m_iModuleSize), (m_iHeight * m_iModuleSize));

	m_iWidth	*= m_iModuleSize;
	m_iHeight	*= m_iModuleSize;
	iLength		= CalcPitch (BIT_COUNT_24, m_iWidth);
	m_uiPMCodeImageSize = iLength * m_iHeight;

	return RESULT_OK;
}

int	CPMCodeEncode::GetPMCodeImageData (char **szImage, UINT *uiImageSize, int *iWidth, int *iHeight) {

	if (m_szPMCodeImage != NULL && m_uiPMCodeImageSize != 0){
		*szImage		= m_szPMCodeImage;
		*uiImageSize	= m_uiPMCodeImageSize;
		*iWidth			= m_iWidth;
		*iHeight		= m_iHeight;
	} else {
		return RESULT_ERROR_ENCODE_YET;
	}
	return RESULT_OK;
}

UINT CPMCodeEncode::GetQRCodeSize (int iLayer) {
	return m_uiQRCodeImageSize [iLayer];
}

int	CPMCodeEncode::GetQRCodeImageData (char **szImage, UINT *uiImageSize, int *iWidth, int *iHeight, int iLayer) {

	if ((m_iLayer - 1 < iLayer) 
		|| (m_szQRCodeImage [iLayer] == NULL) 
		|| (m_uiQRCodeImageSize [iLayer] == 0)) { 
		return RESULT_ERROR_ENCODE_YET;
	}
	*szImage		= m_szQRCodeImage [iLayer];
	*uiImageSize	= m_uiQRCodeImageSize [iLayer];
	*iWidth			= (m_iWidth / m_iModuleSize) - MARGIN;
	*iHeight		= (m_iHeight / m_iModuleSize) - MARGIN;
	return RESULT_OK;
}

