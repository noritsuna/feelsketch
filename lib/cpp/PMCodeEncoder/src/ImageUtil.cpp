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

#include "ImageUtil.h"

ImageUtil::ImageUtil() {
}

ImageUtil::~ImageUtil() {
}

long ImageUtil::CalcPitch (short BitCount,long Width) {

	long lPitch;
	if (BitCount < BIT_COUNT_8) {
		int n = BIT_COUNT_8 / BitCount;
		lPitch = (Width + n - 1) / n;
	} else {
		lPitch = BitCount / 8;
		if (Width % 4 == 0) {
			return Width * lPitch;
		} else {
			return Width * lPitch + (4 - (Width * lPitch) % 4);
		}
	}
	return (lPitch + 3) &~3;
}

BOOL ImageUtil::UpScale (LPBYTE *lpBMP, int iAfterWidth, int iAfterHeight, int iBeforeWidth, int iBeforeHeight) {

	int		iBeforeLength;
	int		iAfterLength;
	int		iMagnification;
	int		iBeforePos;
	int		iAfterPos;
	BYTE	*pbyTemp;
	BYTE	*pBMP;

	pBMP = *lpBMP;
	*lpBMP = NULL;
	if (pBMP == NULL) {
		return FALSE;
	}

	if (iAfterWidth < iBeforeWidth || iAfterHeight < iBeforeHeight) {
		return FALSE;
	}

	iBeforeLength = CalcPitch (BIT_COUNT_24, iBeforeWidth);
	iAfterLength = CalcPitch (BIT_COUNT_24, iAfterWidth);

	if (iAfterWidth != iBeforeWidth) {
		pbyTemp = (LPBYTE)malloc (iBeforeLength * iBeforeHeight + 1);
		if (pbyTemp == NULL) {
			return FALSE;
		}
		memset (pbyTemp, '\0', iBeforeLength * iBeforeHeight + 1);
		memcpy (pbyTemp, pBMP, iBeforeLength * iBeforeHeight);

		free (pBMP);
		pBMP = NULL;
		pBMP = (LPBYTE)malloc (iAfterLength * iBeforeHeight);
		if (pBMP == NULL) {
			free (pbyTemp);
			pbyTemp = NULL;
			return FALSE;
		}
		memset (pBMP, '\0', iAfterLength * iBeforeHeight);
		iMagnification = iAfterWidth / iBeforeWidth;

		for (int i = 0; i < iBeforeHeight; i ++) {
			for (int j = 0; j < iBeforeWidth; j ++) {
				iBeforePos	= (i * iBeforeLength) + (j * 3);
				for (int k = 0; k < iMagnification; k ++) {
					iAfterPos = (i * iAfterLength) + (j * 3 * iMagnification) + (k * 3);
					memcpy (&pBMP [iAfterPos], &pbyTemp [iBeforePos], 3);
				}
			}
		}
		free (pbyTemp);
		pbyTemp = NULL;
	}
	iBeforeLength = iAfterLength;

	if (iAfterHeight != iBeforeHeight) {
		pbyTemp = (LPBYTE)malloc (iAfterLength * iBeforeHeight + 1);
		if (pbyTemp == NULL) {
			return FALSE;
		}
		memset (pbyTemp, '\0', iAfterLength * iBeforeHeight + 1);
		memcpy (pbyTemp, pBMP, iAfterLength * iBeforeHeight);

		free (pBMP);
		pBMP = NULL;
		pBMP = (LPBYTE)malloc (iAfterLength * iAfterHeight + 1);
		if (pBMP == NULL) {
			free (pbyTemp);
			pbyTemp = NULL;
			return FALSE;
		}
		memset (pBMP, '\0', iAfterLength * iAfterHeight + 1);
		iMagnification = iAfterHeight / iBeforeHeight;
		for (int i = 0; i < iAfterWidth; i ++) {
			for (int j = 0; j < iBeforeHeight; j ++) {
				iBeforePos	= (j * iAfterLength) + (i * 3);
				for (int k = 0; k < iMagnification; k ++) {
					iAfterPos = (j * iAfterLength * iMagnification) + (k * iAfterLength) + (i * 3);
					memcpy (&pBMP [iAfterPos], &pbyTemp [iBeforePos], 3);
				}
			}
		}
		free (pbyTemp);
		pbyTemp = NULL;
	}
	*lpBMP = pBMP;
	return TRUE;
}

BOOL ImageUtil::UpDownReplace (LPBYTE lpBMP, int iWidth, int iHeight) {

	int		iLength;
	BYTE	*pbyTemp;

	if (lpBMP == NULL) {
		return FALSE;
	}
	iLength = CalcPitch (BIT_COUNT_24, iWidth);

	pbyTemp = (LPBYTE)malloc (iLength * iHeight + 1);
	if (pbyTemp == NULL) {
		return FALSE;
	}
	memset (pbyTemp, '\0', iLength * iHeight + 1);
	memcpy (pbyTemp, lpBMP, iLength * iHeight);
	memset (lpBMP, '\0', iLength * iHeight);

	for (int i = 0; i < iHeight; i ++) {
		memcpy (&lpBMP [i * iLength], &pbyTemp [(iHeight - (i + 1) )*iLength], iLength);
	}
	free (pbyTemp);
	pbyTemp = NULL;
	return TRUE;
}
