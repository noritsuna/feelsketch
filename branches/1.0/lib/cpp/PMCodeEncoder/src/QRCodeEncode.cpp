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

#include "QRCodeEncode.h"
#include "QRCodeMask.h"
#include "QRCodeSymbol.h"
#include "QRCodeDisposition.h"
#include "global.h"
#include "define.h"

CQRCodeEncode::CQRCodeEncode () {

	m_QRCodeImage		= NULL;
	m_uiQRCodeImageSize	= 0;
	m_szEncodeData		= NULL;
	m_uiEncodeDataSize	= 0;
}

CQRCodeEncode::~CQRCodeEncode () {

	if (m_QRCodeImage != NULL) {
		free (m_QRCodeImage);
		m_QRCodeImage = NULL;
		m_uiQRCodeImageSize = 0;
	}
	if (m_szEncodeData != NULL) {
		free (m_szEncodeData);
		m_szEncodeData = NULL;
		m_uiEncodeDataSize = 0;
	}
}

int	CQRCodeEncode::SetQRCodeData (char * szData, UINT iDataSize, int iVersion, int iRSLevel, int iMaskPattern) {

	int iSymbolSize;
	int iWidthLength;
	int iImageSize;

	iSymbolSize		= VER2SYM (iVersion);
	iWidthLength	= CalcPitch (BIT_COUNT_8, iSymbolSize);
	iImageSize		= iSymbolSize * iWidthLength;

	if ((int)m_uiQRCodeImageSize != iImageSize + 1) {
		m_uiQRCodeImageSize = iImageSize + 1;
		m_QRCodeImage = (char *)malloc (m_uiQRCodeImageSize);
		if (m_QRCodeImage == NULL) {
			return RESULT_ERROR_SECURE_MEMORY;
		} 
	}
	memset (m_QRCodeImage, '\0', m_uiQRCodeImageSize);

	m_uiEncodeDataSize = iDataSize;
	m_szEncodeData = (char *)realloc (m_szEncodeData, m_uiEncodeDataSize + 1);
	if (m_szEncodeData == NULL) {
		return RESULT_ERROR_SECURE_MEMORY;
	} 
	memset (m_szEncodeData, '\0', m_uiEncodeDataSize + 1);
	memcpy (m_szEncodeData, szData, m_uiEncodeDataSize);

	m_iVersion		= iVersion;
	m_iRSLevel		= iRSLevel;
	m_iMaskPattern	= iMaskPattern;
	m_iSymbolSize	= iSymbolSize;
	return RESULT_OK;
}

UINT CQRCodeEncode::EncodeQRCode () {

	int						iRet;
	int						iMinPenalty = 0;
	int						iPenalty	= 0;
	int						iWidthLength;
	int						iWordSize;
	CQRCodeSymbol			*pSymbol;
	CQRCodeMask				*pMask;
	CQRCodeDisposition		*pDisposition;

	SetFunctionModule ();

	pDisposition = new CQRCodeDisposition;
	iWordSize = QR_VersonInfo[m_iVersion].ncAllCodeWord;
	iRet = pDisposition->SetCodeWordPattern (m_szEncodeData, m_uiEncodeDataSize
								, iWordSize
								, m_iSymbolSize, m_iRSLevel);
	delete pDisposition;
	if(RESULT_OK != iRet){
		return 0;
	}
	pMask = new CQRCodeMask;
	if (m_iMaskPattern == -1) {
		m_iMaskPattern = MIN_MASK_NUMBER;
		pMask->SetMaskQRCode (m_iMaskPattern, m_iVersion);
		iMinPenalty = CountPenalty();

		for (int i = 1; i <= MAX_MASK_NUMBER; ++i) {
			pMask->SetMaskQRCode (i, m_iVersion);
			iPenalty = CountPenalty();
			if (iPenalty < iMinPenalty) {
				iMinPenalty = iPenalty;
				m_iMaskPattern = i;
			}
		}
		pMask->SetMaskQRCode (m_iMaskPattern, m_iVersion);
	} else if (m_iMaskPattern <= MAX_MASK_NUMBER) {
		pMask->SetMaskQRCode (m_iMaskPattern, m_iVersion);
	}
	delete pMask;

	pSymbol = new CQRCodeSymbol;
	iRet = pSymbol->SetQRCodeSymbol (m_iSymbolSize, m_iRSLevel, m_iMaskPattern);
	delete pSymbol;
	if(RESULT_OK != iRet){
		return 0;
	}
	iWidthLength = CalcPitch (BIT_COUNT_8, m_iSymbolSize);
	for (int i = 0; i < m_iSymbolSize; i ++) {
		for (int j = 0; j < m_iSymbolSize; j ++) {
			m_QRCodeImage [(i * iWidthLength) + j] = (BYTE)((g_byModuleData[j][m_iSymbolSize - (i + 1)] & 0x11) != 0);
		}
	}
	return m_uiQRCodeImageSize;
}

int	CQRCodeEncode::GetQRCodeImage (char *szImage, UINT iBufferSize) {

	if (m_QRCodeImage != NULL && m_uiQRCodeImageSize != 0) {
		if (iBufferSize >= m_uiQRCodeImageSize) {
			memcpy (szImage, m_QRCodeImage, m_uiQRCodeImageSize);
		} else {
			memcpy (szImage, m_QRCodeImage, iBufferSize);
		}
	} else {
		return RESULT_ERROR_ENCODE_YET;
	}
	return RESULT_OK;
}

void CQRCodeEncode::SetFunctionModule () {

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

void CQRCodeEncode::SetFinderPattern(int x, int y) {

	static BYTE byPattern[] = {0x7f,  // 1111111b
							   0x41,  // 1000001b
							   0x5d,  // 1011101b
							   0x5d,  // 1011101b
							   0x5d,  // 1011101b
							   0x41,  // 1000001b
							   0x7f}; // 1111111b

	for (int i = 0; i < 7; i++)	{
		for (int j = 0; j < 7; j++)	{
			g_byModuleData[x + j][y + i] = (byPattern[i] & (1 << (6 - j))) ? '\x30' : '\x20'; 
		}
	}
}

void CQRCodeEncode::SetVersionPattern() {

	int iVerData;

	if (m_iVersion <= 6) {
		return;
	}
	iVerData = m_iVersion << 12;

	for (int i = 0; i < 6; i ++) {
		if (iVerData & (1 << (17 - i)))	{
			iVerData ^= (0x1f25 << (5 - i));
		}
	}
	iVerData += m_iVersion << 12;
	for (int i = 0; i < 6; i ++) {
		for (int j = 0; j < 3; j ++) {
			g_byModuleData[m_iSymbolSize - 11 + j][i] = g_byModuleData[i][m_iSymbolSize - 11 + j] =
			(iVerData & (1 << (i * 3 + j))) ? '\x30' : '\x20';
		}
	}
}

void CQRCodeEncode::SetAlignmentPattern(int x, int y) {

	static BYTE byPattern[] = {0x1f,  // 11111b
							   0x11,  // 10001b
							   0x15,  // 10101b
							   0x11,  // 10001b
							   0x1f}; // 11111b

	if (g_byModuleData[x][y] & 0x20) {
		return;
	}
	x -= 2; y -= 2;

	for (int i = 0; i < 5; i ++) {
		for (int j = 0; j < 5; j ++) {
			g_byModuleData[x + j][y + i] = (byPattern[i] & (1 << (4 - j))) ? '\x30' : '\x20'; 
		}
	}
}

int CQRCodeEncode::CountPenalty() {

	int iPenalty = 0;
	int iCount = 0;
	int	k = 0;

	for (int i = 0; i < m_iSymbolSize; i ++)	{
		for (int j = 0; j < m_iSymbolSize - 4; j ++) {
			for (k = j + 1, iCount = 1; k < m_iSymbolSize; k++)	{
				if (((g_byModuleData[i][j] & 0x11) == 0) == ((g_byModuleData[i][k] & 0x11) == 0)) {
					iCount ++;
				} else {
					break;
				}
			}
			if (iCount >= 5) {
				iPenalty += 3 + (iCount - 5);
			}
			j = k - 1;
		}
	}

	for (int i = 0; i < m_iSymbolSize; i ++) {
		for (int j = 0; j < m_iSymbolSize - 4; j ++) {
			for (k = j + 1, iCount = 1; k < m_iSymbolSize; k ++) {
				if (((g_byModuleData[j][i] & 0x11) == 0) == ((g_byModuleData[k][i] & 0x11) == 0)) {
					iCount ++;
				} else {
					break;
				}
			}
			if (iCount >= 5) {
				iPenalty += 3 + (iCount - 5);
			}
			j = k - 1;
		}
	}

	for (int i = 0; i < m_iSymbolSize - 1; i ++) {
		for (int j = 0; j < m_iSymbolSize - 1; j ++) {
			if ((((g_byModuleData[i][j] & 0x11) == 0) == ((g_byModuleData[i + 1][j]		& 0x11) == 0)) &&
				(((g_byModuleData[i][j] & 0x11) == 0) == ((g_byModuleData[i]	[j + 1] & 0x11) == 0)) &&
				(((g_byModuleData[i][j] & 0x11) == 0) == ((g_byModuleData[i + 1][j + 1] & 0x11) == 0)))
			{
				iPenalty += 3;
			}
		}
	}

	for (int i = 0; i < m_iSymbolSize; i ++) {
		for (int j = 0; j < m_iSymbolSize - 6; j ++) {
			if (((j == 0) ||				 (! (g_byModuleData[i][j - 1] & 0x11))) &&
											 (   g_byModuleData[i][j]     & 0x11)   &&
											 (! (g_byModuleData[i][j + 1] & 0x11))  &&
											 (   g_byModuleData[i][j + 2] & 0x11)   &&
											 (   g_byModuleData[i][j + 3] & 0x11)   &&
											 (   g_byModuleData[i][j + 4] & 0x11)   &&
											 (! (g_byModuleData[i][j + 5] & 0x11))  &&
											 (   g_byModuleData[i][j + 6] & 0x11)   &&
				((j == m_iSymbolSize - 7) || (! (g_byModuleData[i][j + 7] & 0x11))))
			{
				if (((j < 2 || ! (g_byModuleData[i][j - 2] & 0x11)) && 
					 (j < 3 || ! (g_byModuleData[i][j - 3] & 0x11)) &&
					 (j < 4 || ! (g_byModuleData[i][j - 4] & 0x11))) ||
					((j >= m_iSymbolSize - 8  || ! (g_byModuleData[i][j + 8]  & 0x11)) &&
					 (j >= m_iSymbolSize - 9  || ! (g_byModuleData[i][j + 9]  & 0x11)) &&
					 (j >= m_iSymbolSize - 10 || ! (g_byModuleData[i][j + 10] & 0x11))))
				{
					iPenalty += 40;
				}
			}
		}
	}

	for (int i = 0; i < m_iSymbolSize; i ++) {
		for (int j = 0; j < m_iSymbolSize - 6; j ++) {
			if (((j == 0) ||				 (! (g_byModuleData[j - 1][i] & 0x11))) &&
											 (   g_byModuleData[j]    [i] & 0x11)   &&
											 (! (g_byModuleData[j + 1][i] & 0x11))  &&
											 (   g_byModuleData[j + 2][i] & 0x11)   &&
											 (   g_byModuleData[j + 3][i] & 0x11)   &&
											 (   g_byModuleData[j + 4][i] & 0x11)   &&
											 (! (g_byModuleData[j + 5][i] & 0x11))  &&
											 (   g_byModuleData[j + 6][i] & 0x11)   &&
				((j == m_iSymbolSize - 7) || (! (g_byModuleData[j + 7][i] & 0x11))))
			{
				if (((j < 2 || ! (g_byModuleData[j - 2][i] & 0x11)) && 
					 (j < 3 || ! (g_byModuleData[j - 3][i] & 0x11)) &&
					 (j < 4 || ! (g_byModuleData[j - 4][i] & 0x11))) ||
					((j >= m_iSymbolSize - 8  || ! (g_byModuleData[j + 8][i]  & 0x11)) &&
					 (j >= m_iSymbolSize - 9  || ! (g_byModuleData[j + 9][i]  & 0x11)) &&
					 (j >= m_iSymbolSize - 10 || ! (g_byModuleData[j + 10][i] & 0x11))))
				{
					iPenalty += 40;
				}
			}
		}
	}

	for (int i = 0, iCount = 0; i < m_iSymbolSize; i ++) {
		for (int j = 0; j < m_iSymbolSize; j ++) {
			if (! (g_byModuleData[i][j] & 0x11)) {
				iCount ++;
			}
		}
	}
	iPenalty += (abs(50 - ((iCount * 100) / (m_iSymbolSize * m_iSymbolSize))) / 5) * 10;
	return iPenalty;
}
