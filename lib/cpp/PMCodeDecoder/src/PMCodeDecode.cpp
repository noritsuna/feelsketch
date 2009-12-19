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

#include "PMCodeDecode.h"
#include "QRCodeDecode.h"
#include "global.h"
#include "define.h"
#include "ReedSolomon.h"
#include "mathematics.h"


CPMCodeDecode::CPMCodeDecode () {

	m_szBaseImage		= NULL;
	m_uiBaseImageSize	= 0;
	m_szDecodeData		= NULL;
	m_uiDecodeDataSize	= 0;
	m_Width				= 0;
	m_Height			= 0;
	m_szQRCodeImage		= NULL;
	m_uiQRCodeImageSize	= 0;
}

CPMCodeDecode::~CPMCodeDecode () {

	if (m_szBaseImage != NULL) {
		free (m_szBaseImage);
		m_szBaseImage = NULL;
		m_uiBaseImageSize = 0;
	}
	if (m_szDecodeData != NULL) {
		free (m_szDecodeData);
		m_szDecodeData = NULL;
		m_uiDecodeDataSize = 0;
	}
	if (m_szQRCodeImage != NULL){
		free (m_szQRCodeImage);
		m_szQRCodeImage = NULL;
	}
}

void CPMCodeDecode::SetBaseColor (int *baseColors, int layer) {
	int i;
	for (i = 0; i < layer; i++) {
		g_ColorCodeSettingDefinition[i] = baseColors[i];
	}
}

int CPMCodeDecode::SetBaseImage (char *szImage, UINT uiImageSize, int iWidth, int iHeight) {

	if (m_szBaseImage != NULL) {
		free (m_szBaseImage);
		m_szBaseImage = NULL;
	}
	m_szBaseImage = (char *)realloc (m_szBaseImage, uiImageSize);
	if (m_szBaseImage == NULL) {
		return RESULT_ERROR_SECURE_MEMORY;
	} 
	memset (m_szBaseImage, '\0', uiImageSize);
	memcpy (m_szBaseImage, szImage, uiImageSize);
	m_uiBaseImageSize = uiImageSize;
	m_Width		= iWidth;
	m_Height	= iHeight;
	return RESULT_OK;
}

int CPMCodeDecode::DecodePMCode () {

	CQRCodeDecode	*pQRCodeDecode						= NULL;
	UINT			uiDecodeDataSize;
	char			*szQRCodeImage = NULL;
	int				iQRCodeImageSize;
	int				iQRCodeLengthSize;
	long			lColorCode;
	int				iRet;
	CReedSolomon	cReedSolomon;
	BOOL			bRet;
	int				iDataSize;
	int				iBlockSize;
	int				iCorrectPos = 0;
	int				iCorrectDataSize;
	int				iRSBlock1;
	int				iRSBlock2;
	BYTE			szCorrectBuf [256];
	BYTE			szWork [256];
	char			*szDecodeData = NULL;

	if (m_szDecodeData != NULL) {
		free (m_szDecodeData);
		m_szDecodeData = NULL;
		m_uiDecodeDataSize = 0;
	}

	if (m_szBaseImage == NULL || m_uiBaseImageSize == 0) {
		return RESULT_ERROR_IMAGEFILE_READ_YET;
	}

	m_iVersion		= SYM2VER(m_Width);
	m_iSymbolSize	= m_Width;

	iRet = GetColorCode (g_PMCodeColor, m_szBaseImage, m_uiBaseImageSize, m_iSymbolSize, m_iVersion);
	if(RESULT_OK != iRet){
		return iRet;
	}

	m_iLayer = GetLayerCount (m_szBaseImage, m_uiBaseImageSize, m_iSymbolSize);

	if(0 == m_iLayer){
		return RESULT_ERROR_ANALYSIS_LAYER;
	}

	if (m_iVersion < 3) {
		MakeColorTable (3, m_iLayer);
	} else {
		MakeColorTable (6, m_iLayer);
	}

	iQRCodeLengthSize	= CalcPitch (BIT_COUNT_8, m_iSymbolSize);
	iQRCodeImageSize	= iQRCodeLengthSize * m_iSymbolSize;

	if (szQRCodeImage != NULL){
		free (szQRCodeImage);
		szQRCodeImage = NULL;
	}
	szQRCodeImage = (char *)malloc (iQRCodeImageSize + 1);
	if (szQRCodeImage == NULL) {
		return RESULT_ERROR_SECURE_MEMORY;
	}
	memset (szQRCodeImage, '\0', iQRCodeImageSize + 1);				
	pQRCodeDecode = new CQRCodeDecode;

	m_szDecodeData = (char *)realloc (m_szDecodeData, (QR_VersonInfo[m_iVersion].ncAllCodeWord * m_iLayer) + 1);
	memset (m_szDecodeData, '\0', (QR_VersonInfo[m_iVersion].ncAllCodeWord * m_iLayer) + 1);

	for (int i = 0; i < m_iLayer; i ++ ) {
#if PM_CODE_COLOR
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
#else
		lColorCode = g_PMCodeColor [i];
#endif
		if (lColorCode == 0) {
			delete pQRCodeDecode;
			pQRCodeDecode = NULL;
			free (szQRCodeImage);
			szQRCodeImage = NULL;
			return RESULT_ERROR_NON_SUPORT_COLOR;
		}
		iRet = PMCodeToQRCode (m_szBaseImage, m_uiBaseImageSize, lColorCode
										, szQRCodeImage, iQRCodeImageSize, m_iSymbolSize);
		if(RESULT_OK != iRet){
			free (szQRCodeImage);
			szQRCodeImage = NULL;
			return RESLUT_ERROR_LAYER_RESOLUTION;
		}

		iRet = pQRCodeDecode->SetQRCodeImage (szQRCodeImage, iQRCodeImageSize, m_iVersion);
		if(RESULT_OK != iRet){
			free (szQRCodeImage);
			szQRCodeImage = NULL;
			delete pQRCodeDecode;
			pQRCodeDecode = NULL;
			return RESULT_ERROR_SECURE_MEMORY;
		}

		uiDecodeDataSize = pQRCodeDecode->DecodeQRCodeImage ();
		if(0 == uiDecodeDataSize){
			free (szQRCodeImage);
			szQRCodeImage = NULL;
			delete pQRCodeDecode;
			pQRCodeDecode = NULL;
			return RESULT_ERROR_ANALYSIS_QR_IMAGE;
		}

		m_iMaskPattern [i] = pQRCodeDecode->GetMaskNumber ();
		iRet = pQRCodeDecode->GetQRCodeStatus (&m_iRSLevel);
		if(RESULT_OK != iRet){
			free (szQRCodeImage);
			szQRCodeImage = NULL;
			delete pQRCodeDecode;
			pQRCodeDecode = NULL;
			return RESULT_ERROR_ANALYSIS_QR_STATUS;
		}

		if (szDecodeData != NULL) {
			free (szDecodeData);
			szDecodeData = NULL;
		}
		szDecodeData = (char *)malloc (uiDecodeDataSize + 1);
		memset (szDecodeData, '\0', uiDecodeDataSize + 1);
		iRet = pQRCodeDecode->GetDecodeData (szDecodeData, uiDecodeDataSize);

		if(RESULT_OK != iRet){
			free (szQRCodeImage);
			szQRCodeImage = NULL;
			delete pQRCodeDecode;
			pQRCodeDecode = NULL;
			return RESULT_ERROR_ANALYSIS_QR_DECODE;
		}

		iDataSize = uiDecodeDataSize;
		iCorrectPos = 0;
		iRSBlock1 = QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncRSBlock;
		iRSBlock2 = QR_VersonInfo[m_iVersion].RS_BlockInfo2 [m_iRSLevel].ncRSBlock;

		for (int j = 0; j < (iRSBlock1 + iRSBlock2); j ++) {
			if (QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncRSBlock > j){
				iBlockSize = QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncAllCodeWord;
				cReedSolomon.SetCorrectCodeSize (iBlockSize	- QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncDataCodeWord);
			} else {
				iBlockSize = QR_VersonInfo[m_iVersion].RS_BlockInfo2 [m_iRSLevel].ncAllCodeWord;
				cReedSolomon.SetCorrectCodeSize (iBlockSize - QR_VersonInfo[m_iVersion].RS_BlockInfo2 [m_iRSLevel].ncDataCodeWord);
			}
			memset (szWork, NULL, sizeof (szWork));
			memset (szCorrectBuf, NULL, sizeof (szCorrectBuf));

			if (iDataSize < iBlockSize) {
				memcpy (szWork, &szDecodeData [iCorrectPos], iDataSize);
				bRet = cReedSolomon.Decode (szWork, iBlockSize, szCorrectBuf, &iCorrectDataSize);
				if(bRet != TRUE){
					free (szQRCodeImage);
					szQRCodeImage = NULL;
					delete pQRCodeDecode;
					pQRCodeDecode = NULL;
					free (szDecodeData);
					szDecodeData = NULL;
					return RESULT_ERROR_ANALYSIS_QR_BIG_BLOCK_DECODE;
				}
			} else {
				memcpy (szWork, &szDecodeData [iCorrectPos], iBlockSize);
				bRet = cReedSolomon.Decode (szWork, iBlockSize, szCorrectBuf, &iCorrectDataSize);
				if(bRet != TRUE){
					free (szQRCodeImage);
					szQRCodeImage = NULL;
					delete pQRCodeDecode;
					pQRCodeDecode = NULL;
					free (szDecodeData);
					szDecodeData = NULL;
					return RESULT_ERROR_ANALYSIS_QR_SMALL_BLOCK_DECODE;
				}
			}
			memcpy (&m_szDecodeData [m_uiDecodeDataSize], szCorrectBuf, iCorrectDataSize);
			m_uiDecodeDataSize += iCorrectDataSize;
			iCorrectPos += iBlockSize;
			iDataSize -= iBlockSize;
		}
		if (szDecodeData != NULL) {
			free (szDecodeData);
			szDecodeData = NULL;
		}
	}
	delete pQRCodeDecode;
	pQRCodeDecode = NULL;

	if (szQRCodeImage != NULL) {
		free (szQRCodeImage);
		szQRCodeImage = NULL;
	}
	return m_uiDecodeDataSize;
}

int CPMCodeDecode::GetPMCodeDecodeData (char *szData, UINT uiBufferSize) {

	if (m_szDecodeData != NULL && m_uiDecodeDataSize != 0) {
		if (m_uiDecodeDataSize >= uiBufferSize) {
			memcpy (szData, m_szDecodeData, uiBufferSize);
		} else {
			memcpy (szData, m_szDecodeData, m_uiDecodeDataSize);
		}
	} else {
		return RESULT_ERROR_DECODE_YET;
	}
	return RESULT_OK;
}

UINT CPMCodeDecode::GetQRCodeSize (int iLayer) {
	return m_Width * m_Height;
}

int CPMCodeDecode::GetQRCodeImage (char ** szImage, UINT *uiImageSize, int *iWidth, int *iHeight, int iLayer) {

	int		iQRCodeLengthSize;
	long	lColorCode;
	int		iRet;

	if (m_szQRCodeImage != NULL && m_uiQRCodeImageSize != 0){
#if PM_CODE_COLOR
		if (g_ColorCodeSetting == 0) {
			lColorCode = g_ColorCodeSettingDefinition [iLayer];	
		} else {
			if (m_iLayer == 3) {
				lColorCode = g_ColorCodeSetting3 [iLayer];	
			} else if (m_iLayer >9) {
				lColorCode = g_ColorCodeSetting10to24 [iLayer];	
			} else {
				lColorCode = g_ColorCodeSetting4to9 [iLayer];	
			}
		}
#else
		lColorCode = g_PMCodeColor [iLayer];
#endif
		if (lColorCode == 0) {
			return RESULT_ERROR_NON_SUPORT_COLOR;
		}
		iQRCodeLengthSize	= CalcPitch (BIT_COUNT_8, m_iSymbolSize);
		m_uiQRCodeImageSize	= iQRCodeLengthSize * m_iSymbolSize;

		if (m_szQRCodeImage != NULL){
			free (m_szQRCodeImage);
			m_szQRCodeImage = NULL;
		}
		m_szQRCodeImage = (char *)malloc (m_uiQRCodeImageSize + 1);
		if (m_szQRCodeImage == NULL) {
			return 0;
		}
		memset (m_szQRCodeImage, '\0', m_uiQRCodeImageSize + 1);

		iRet = PMCodeToQRCode (m_szBaseImage, m_uiBaseImageSize, lColorCode
										, m_szQRCodeImage, m_uiQRCodeImageSize, m_Width);
		if(RESULT_OK != iRet){
			return iRet;
		}
		*szImage		= m_szQRCodeImage;
		*uiImageSize	= m_uiQRCodeImageSize;
		*iWidth			= m_Width;
		*iHeight		= m_Height;
	} else {
		return RESULT_ERROR_DECODE_YET;
	}
	return RESULT_OK;
}


