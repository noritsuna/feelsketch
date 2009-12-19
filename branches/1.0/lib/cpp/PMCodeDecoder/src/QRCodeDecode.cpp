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

#include "QRCodeDecode.h"
#include "QRCodeMask.h"
#include "QRCodeSymbol.h"
#include "QRCodeRestoration.h"
#include "global.h"
#include "define.h"


CQRCodeDecode::CQRCodeDecode () {

	m_QRCodeImage		= NULL;
	m_uiQRCodeImageSize	= 0;
	m_szDecodeData		= NULL;
	m_uiDecodeDataSize	= 0;
}

CQRCodeDecode::~CQRCodeDecode () {

	if (m_QRCodeImage != NULL) {
		free (m_QRCodeImage);
		m_QRCodeImage = NULL;
		m_uiQRCodeImageSize = 0;
	}
	if (m_szDecodeData != NULL) {
		free (m_szDecodeData);
		m_szDecodeData = NULL;
		m_uiDecodeDataSize = 0;
	}
}

int	CQRCodeDecode::SetQRCodeImage (char * szImage, UINT iImageSize, int iVersion) {

	if (m_QRCodeImage == NULL) {
		m_QRCodeImage = (char *)malloc (iImageSize + 1);
		if (m_QRCodeImage == NULL) {
			return RESULT_ERROR_SECURE_MEMORY;
		} 
	}		

	memset (m_QRCodeImage, '\0', iImageSize + 1);
	memcpy (m_QRCodeImage, szImage, iImageSize);
	m_uiQRCodeImageSize = iImageSize;
	m_iVersion			= iVersion;
	m_iSymbolSize		= VER2SYM (m_iVersion);
	return RESULT_OK;
}

UINT CQRCodeDecode::DecodeQRCodeImage () {

	int						iRet;
	int						iWordSize;
	CQRCodeSymbol			*pSymbol;
	CQRCodeMask				*pMask;
	CQRCodeRestoration		*pRestoration	

	SetFunctionModule ();

	pSymbol = new CQRCodeSymbol;
	iRet = pSymbol->GetQRCodeSymbol (m_QRCodeImage, m_uiQRCodeImageSize, m_iSymbolSize
								, &m_iRSLevel, &m_iMaskPattern);
	delete pSymbol;
	if(RESULT_OK != iRet){
		return 0;
	}

	pMask = new CQRCodeMask;
	pMask->GetMaskQRCode (m_QRCodeImage, m_uiQRCodeImageSize, m_iMaskPattern, m_iVersion);
	delete pMask;

	pRestoration = new CQRCodeRestoration;

	iWordSize = QR_VersonInfo[m_iVersion].ncAllCodeWord;

	iRet = pRestoration->SetCodeWordPattern (m_QRCodeImage, m_uiQRCodeImageSize
											, iWordSize, m_iSymbolSize, m_iRSLevel);


	if(RESULT_OK != iRet){
		delete pRestoration;
		return 0;
	}
	if (m_szDecodeData != NULL) {
		free (m_szDecodeData);
		m_szDecodeData = NULL;
		m_uiDecodeDataSize = 0;
	}
	m_uiDecodeDataSize = iWordSize;
	m_szDecodeData = (char *)malloc (m_uiDecodeDataSize + 1);
	if (m_szDecodeData == NULL) {
		delete pRestoration;
		return 0;
	}
	memset (m_szDecodeData, '\0', m_uiDecodeDataSize + 1);

	iRet = pRestoration->GetCodeData (m_szDecodeData, m_uiDecodeDataSize);
	if(RESULT_OK != iRet){
		delete pRestoration;
		return 0;
	}
	delete pRestoration;
	return m_uiDecodeDataSize;
}

int	CQRCodeDecode::GetDecodeData (char *szData, UINT iBufferSize) {

	if (m_szDecodeData != NULL && m_uiDecodeDataSize != 0) {
		if (iBufferSize >= m_uiDecodeDataSize) {
			memcpy (szData, m_szDecodeData, m_uiDecodeDataSize);
		} else {
			memcpy (szData, m_szDecodeData, iBufferSize);
		}
	} else {
		return RESULT_ERROR_DECODE_YET;
	}
	return RESULT_OK;
}

int	CQRCodeDecode::GetQRCodeStatus (int * iRSLevel) {

	if (m_szDecodeData != NULL && m_uiDecodeDataSize != 0) {
		*iRSLevel		= m_iRSLevel;
	} else {
		return RESULT_ERROR_DECODE_YET;
	}
	return RESULT_OK;
}

int	CQRCodeDecode::GetMaskNumber () {
	return m_iMaskPattern;
}


void CQRCodeDecode::SetFunctionModule () {

	memset (g_byModuleData, '\0', sizeof (g_byModuleData));

	SetFinderPattern(0, 0);
	SetFinderPattern(m_iSymbolSize - 7, 0);
	SetFinderPattern(0, m_iSymbolSize - 7);

	for (int i = 0; i < 8; i ++) {
		g_byModuleData[i][7]					= g_byModuleData[7][i] = '\x20';
		g_byModuleData[m_iSymbolSize - 8][i]	= g_byModuleData[m_iSymbolSize - 8 + i][7] = '\x20';
		g_byModuleData[i][m_iSymbolSize - 8]	= g_byModuleData[7][m_iSymbolSize - 8 + i] = '\x20';
	}

	for (int i = 0; i < 9; i ++) {
		g_byModuleData[i][8] = g_byModuleData[8][i] = '\x20';
	}

	for (int i = 0; i < 8; i ++) {
		g_byModuleData[m_iSymbolSize - 8 + i][8] = g_byModuleData[8][m_iSymbolSize - 8 + i] = '\x20';
	}

	for (int i = 0; i < QR_VersonInfo[m_iVersion].ncAlignPoint; i ++) {
		SetAlignmentPattern(QR_VersonInfo[m_iVersion].nAlignPoint[i], 6);
		SetAlignmentPattern(6, QR_VersonInfo[m_iVersion].nAlignPoint[i]);

		for (int j = 0; j < QR_VersonInfo[m_iVersion].ncAlignPoint; j ++) {
			SetAlignmentPattern(QR_VersonInfo[m_iVersion].nAlignPoint[i]
				, QR_VersonInfo[m_iVersion].nAlignPoint[j]);
		}
	}

	for (int i = 8; i <= m_iSymbolSize - 9; i ++) {
		g_byModuleData[i][6] = (i % 2) == 0 ? '\x30' : '\x20';
		g_byModuleData[6][i] = (i % 2) == 0 ? '\x30' : '\x20';
	}

#if 0
	for (int i = 0; i <= 5; i ++) {
		for (int j = 9; j <= 12; j ++) {
			g_byModuleData[i][j] = '\x20';
		}
	}
#else
	int iEnd;

	if (m_iVersion < 3){
		iEnd = 12;
	} else {
		iEnd = 20;
	}
	for (int i = 0; i <= 5; i ++) {
		for (int j = 9; j <= iEnd; j ++) {
			g_byModuleData[i][j] = '\x20';
		}
	}
#endif
}

void CQRCodeDecode::SetFinderPattern(int x, int y) {

	static BYTE byPattern[] = {0x7f,  // 1111111b
							   0x41,  // 1000001b
							   0x5d,  // 1011101b
							   0x5d,  // 1011101b
							   0x5d,  // 1011101b
							   0x41,  // 1000001b
							   0x7f}; // 1111111b

	for (int i = 0; i < 7; ++i)	{
		for (int j = 0; j < 7; ++j)	{
			g_byModuleData[x + j][y + i] = (byPattern[i] & (1 << (6 - j))) ? '\x30' : '\x20'; 
		}
	}
}

void CQRCodeDecode::SetVersionPattern() {

	if (m_iVersion <= 6) {
		return;
	}
	int nVerData = m_iVersion << 12;

	for (int i = 0; i < 6; ++i)
	{
		if (nVerData & (1 << (17 - i)))
		{
			nVerData ^= (0x1f25 << (5 - i));
		}
	}

	nVerData += m_iVersion << 12;

	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			g_byModuleData[m_iSymbolSize - 11 + j][i] = g_byModuleData[i][m_iSymbolSize - 11 + j] =
			(nVerData & (1 << (i * 3 + j))) ? '\x30' : '\x20';
		}
	}
}

void CQRCodeDecode::SetAlignmentPattern(int x, int y) {

	static BYTE byPattern[] = {0x1f,  // 11111b
							   0x11,  // 10001b
							   0x15,  // 10101b
							   0x11,  // 10001b
							   0x1f}; // 11111b

	if (g_byModuleData[x][y] & 0x20) {
		return;
	}
	x -= 2; y -= 2;

	for (int i = 0; i < 5; ++i)	{
		for (int j = 0; j < 5; ++j)	{
			g_byModuleData[x + j][y + i] = (byPattern[i] & (1 << (4 - j))) ? '\x30' : '\x20'; 
		}
	}
}

