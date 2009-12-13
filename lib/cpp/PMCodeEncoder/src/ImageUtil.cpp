/*
 * ImageUtil.cpp
 *
 *  Created on: 2009/11/17
 *      Author: gabu
 */

#include "ImageUtil.h"

ImageUtil::ImageUtil() {
}

ImageUtil::~ImageUtil() {
}

// ------------------------------------------------------------------------- //
// 機能概要			:１行のビット長計算										 //
// 引数				:BitCount			:ビットカウント(8,16,24,32のみ)		 //
//					:Width				:幅									 //
// 戻り値			:１行のビット長											 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
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

// ------------------------------------------------------------------------- //
// 機能概要			:ビットマップの拡大処理									 //
// 引数				:lpBMP				:画像データ							 //
//					:iAfterWidth		:拡大処理後幅						 //
//					:iAfterHeight		:拡大処理後高さ						 //
//					:iBeforeWidth		:拡大処理前幅						 //
//					:iBeforeHeight		:拡大処理前高さ						 //
// 戻り値			:TRUE:成功　FALSE:失敗									 //
// 備考				:２４ビットカラーのビットマップのみ対応					 //
//					:拡大率が整数となる様画像の大きさに注意する				 //
// ------------------------------------------------------------------------- //
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
	if (pBMP == NULL) {														// 画像バッファ未確保
		return FALSE;
	}

	// 拡大サイズが元のビットマップサイズより小さい場合はエラー
	if (iAfterWidth < iBeforeWidth || iAfterHeight < iBeforeHeight) {
		return FALSE;
	}

	iBeforeLength = CalcPitch (BIT_COUNT_24, iBeforeWidth);
	iAfterLength = CalcPitch (BIT_COUNT_24, iAfterWidth);

//	横方向の拡大
	if (iAfterWidth != iBeforeWidth) {										// 倍率が一致しない時のみ実施
		// 作業領域の再確保を行い、作業領域にコピー
		pbyTemp = (LPBYTE)malloc (iBeforeLength * iBeforeHeight + 1);
		if (pbyTemp == NULL) {
			return FALSE;
		}
		memset (pbyTemp, '\0', iBeforeLength * iBeforeHeight + 1);
		memcpy (pbyTemp, pBMP, iBeforeLength * iBeforeHeight);

		// 画像データ領域の再確保を行う
		free (pBMP);
		pBMP = NULL;
		pBMP = (LPBYTE)malloc (iAfterLength * iBeforeHeight);
		if (pBMP == NULL) {													// 領域確保失敗
			free (pbyTemp);
			pbyTemp = NULL;
			return FALSE;
		}
		memset (pBMP, '\0', iAfterLength * iBeforeHeight);					// 領域の初期化
		iMagnification = iAfterWidth / iBeforeWidth;						// 横方向の拡大率

		for (int i = 0; i < iBeforeHeight; i ++) {
			for (int j = 0; j < iBeforeWidth; j ++) {						// 横拡大後の高さ
				iBeforePos	= (i * iBeforeLength) + (j * 3);
				for (int k = 0; k < iMagnification; k ++) {
					iAfterPos = (i * iAfterLength) + (j * 3 * iMagnification) + (k * 3);
					memcpy (&pBMP [iAfterPos], &pbyTemp [iBeforePos], 3);
				}
			}
		}
		free (pbyTemp);														// 作業領域の開放
		pbyTemp = NULL;
	}
	iBeforeLength = iAfterLength;

// 縦方向の拡大
	if (iAfterHeight != iBeforeHeight) {									// 倍率が一致しない時のみ実施
		// 作業領域の確保を行い、作業領域にコピー
		pbyTemp = (LPBYTE)malloc (iAfterLength * iBeforeHeight + 1);
		if (pbyTemp == NULL) {												// 領域確保失敗
			return FALSE;
		}
		memset (pbyTemp, '\0', iAfterLength * iBeforeHeight + 1);
		memcpy (pbyTemp, pBMP, iAfterLength * iBeforeHeight);

		// 画像データ領域の再確保を行う
		free (pBMP);
		pBMP = NULL;
		pBMP = (LPBYTE)malloc (iAfterLength * iAfterHeight + 1);			// 拡大後に
		if (pBMP == NULL) {													// 領域確保失敗
			free (pbyTemp);
			pbyTemp = NULL;
			return FALSE;
		}
		memset (pBMP, '\0', iAfterLength * iAfterHeight + 1);				// 領域の初期化
		iMagnification = iAfterHeight / iBeforeHeight;						// 縦方向の拡大率
		for (int i = 0; i < iAfterWidth; i ++) {
			for (int j = 0; j < iBeforeHeight; j ++) {						// 拡大後の高さ分となる
				iBeforePos	= (j * iAfterLength) + (i * 3);
				for (int k = 0; k < iMagnification; k ++) {
					iAfterPos = (j * iAfterLength * iMagnification) + (k * iAfterLength) + (i * 3);
					memcpy (&pBMP [iAfterPos], &pbyTemp [iBeforePos], 3);
				}
			}
		}
		free (pbyTemp);														// 作業領域を開放
		pbyTemp = NULL;
	}
	*lpBMP = pBMP;
	return TRUE;
}

// ------------------------------------------------------------------------- //
// 機能概要			:ビットマップの上下入れ替え処理							 //
// 引数				:lpBMP				:画像データ							 //
//					:iWidth				:幅									 //
//					:iHeight			:高さ								 //
// 戻り値			:TRUE:成功　FALSE:失敗									 //
// 備考				:２４ビットカラーのビットマップのみ対応					 //
// ------------------------------------------------------------------------- //
BOOL ImageUtil::UpDownReplace (LPBYTE lpBMP, int iWidth, int iHeight) {

	int		iLength;
	BYTE	*pbyTemp;

	if (lpBMP == NULL) {													// 画像バッファ未確保
		return FALSE;
	}
	iLength = CalcPitch (BIT_COUNT_24, iWidth);

	// 作業領域の再確保を行い、作業領域にコピー
	pbyTemp = (LPBYTE)malloc (iLength * iHeight + 1);
	if (pbyTemp == NULL) {
		return FALSE;
	}
	memset (pbyTemp, '\0', iLength * iHeight + 1);
	memcpy (pbyTemp, lpBMP, iLength * iHeight);
	memset (lpBMP, '\0', iLength * iHeight);								// 領域の初期化

	for (int i = 0; i < iHeight; i ++) {
		memcpy (&lpBMP [i * iLength], &pbyTemp [(iHeight - (i + 1) )*iLength], iLength);
	}
	free (pbyTemp);															// 作業領域の開放
	pbyTemp = NULL;
	return TRUE;
}
