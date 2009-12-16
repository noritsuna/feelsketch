#include "PMCodeToQRCodes.h"
#include "Global.h"
#include "define.h"
#include <stdio.h>




// ------------------------------------------------------------------------- //
// 機能概要			:コンストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CPMCodeToQRCodes::CPMCodeToQRCodes () {

}

// ------------------------------------------------------------------------- //
// 機能概要			:デストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CPMCodeToQRCodes::~CPMCodeToQRCodes () {

}

// ------------------------------------------------------------------------- //
// 機能概要			:ＰＭコード→ＱＲコード変換								 //
// 引数				:szPMCodeImage		:ＰＭコード画像データ				 //
//                  :uiPMCodeImageSize	:ＰＭコード画像データサイズ			 //
//					:lColorCode			:カラーコード						 //
//					:szQRCodeImage		:ＱＲコード画像データ				 //
//                  :uiQRCodeImageSize	:ＱＲコード画像データサイズ			 //
//                  :uiSymbolSize		:ＱＲコードシンボルサイズ			 //
// 戻り値			:RESULT_OK												 //
// 備考				:ＱＲコードのバッファは予め確保しておくこと				 //
// ------------------------------------------------------------------------- //
int CPMCodeToQRCodes::PMCodeToQRCode (char *szPMCodeImage, UINT uiPMCodeImageSize, long lColorCode
									, char *szQRCodeImage, UINT uiQRCodeImageSize, int uiSymbolSize) {
	long	lBit;
	int		iBaseImagePos;
	int		iValueImagePos;
	int		iBaseWidthLength;
	int		iValueWidthLength;
	int		iBitSize;
	char	szColor [8];
	char	szTemp	[17];
	char	cTemp;

	iBitSize	= BIT_COUNT_24 / 8;

	iBaseWidthLength = CalcPitch (BIT_COUNT_24, uiSymbolSize);
	iValueWidthLength = CalcPitch (BIT_COUNT_8, uiSymbolSize);

	if (uiPMCodeImageSize < ((unsigned int)iBaseWidthLength * uiSymbolSize)) {
		return RESULT_ERROR_SECURE_MEMORY;	
	}
	if (uiQRCodeImageSize < ((unsigned int)iValueWidthLength * uiSymbolSize)) {
		return RESULT_ERROR_SECURE_MEMORY;	
	}
	// カラーコードの入れ替え
	// 画面及び、INIファイル上ではRGBだが、画像データなどはBGRで格納されているので、入れ替えを行う
	memset (szTemp, '\0', sizeof (szTemp));
	sprintf (szTemp, "%0.6X", lColorCode);

	cTemp = szTemp [0];
	szTemp [0] = szTemp [4];
	szTemp [4] = cTemp;
	cTemp = szTemp [1];
	szTemp [1] = szTemp [5];
	szTemp [5] = cTemp;
	lColorCode = AscHexToLong(szTemp, strlen (szTemp));

	// 単層分解処理
	for (int i = 0; i < uiSymbolSize; i ++) {
		for (int j = 0; j < uiSymbolSize; j ++) {
			// 元画像の検出位置算出
			iBaseImagePos = (i * iBaseWidthLength) + (j * iBitSize);

			// 三値画像の設定位置算出
			iValueImagePos = (i * iValueWidthLength) + j;

			memset (szColor, '\0', sizeof (szColor));
			memcpy (szColor, &szPMCodeImage [iBaseImagePos], iBitSize);

			memset (szTemp, '\0', sizeof (szTemp));
			for (int k = 0 ; k < iBitSize; k ++) {
				b1th2 (szColor [k], &szTemp [k * 2]);
			}
			lBit = AscHexToLong (szTemp, iBitSize * 2);

			// カラーコードでANDをかけた結果によりビットのON/OFFを決める
			if (lBit == 0xFFFFFF) {
				szQRCodeImage [iValueImagePos] = BIT_ON;					// 黒
//			if (lBit == 0xFFFFFF) {
//				szQRCodeImage [iValueImagePos] = BIT_OFF;					// 白
			} else if (lBit == 0x000000) {
				szQRCodeImage [iValueImagePos] = BIT_OFF;					// 白
//			} else if (lBit == 0x000000) {
//				szQRCodeImage [iValueImagePos] = BIT_ON;					// 黒
			} else if ((lBit & lColorCode) == 0x000000){
				szQRCodeImage [iValueImagePos] = BIT_OFF;					// 白
			} else {
				szQRCodeImage [iValueImagePos] = BIT_ON;					// 黒
			}
		}
	}
	return RESULT_OK;
}
