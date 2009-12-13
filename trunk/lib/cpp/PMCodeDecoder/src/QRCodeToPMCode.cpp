#include "QRCodeToPMCode.h"
#include "global.h"
#include "define.h"
#include "stdio.h"

// ------------------------------------------------------------------------- //
// 機能概要			:コンストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CQRCodeToPMCode::CQRCodeToPMCode () {

}

// ------------------------------------------------------------------------- //
// 機能概要			:デストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CQRCodeToPMCode::~CQRCodeToPMCode () {

}

// ------------------------------------------------------------------------- //
// 機能概要			:ＱＲコード→ＰＭコード変換								 //
// 引数				:szQRCodeImage		:ＱＲコード画像データ				 //
//                  :uiQRCodeImageSize	:ＱＲコード画像データサイズ			 //
//					:lColorCode			:カラーコード						 //
//					:szPMCodeImage		:ＰＭコード画像データ				 //
//                  :uiPMCodeImageSize	:ＰＭコード画像データサイズ			 //
//                  :uiSymbolSize		:シンボルサイズ						 //
// 戻り値			:RESULT_OK												 //
// 備考				:ＰＭコードのバッファは予め確保しておくこと				 //
//					:引数にて渡されたＰＭコードに色を追加していくので		 //
//					:バッファはクリアしない事！								 //
// ------------------------------------------------------------------------- //
int CQRCodeToPMCode::QRCodeToPMCode (char *szQRCodeImage, UINT uiQRCodeImageSize, long lColorCode
							, char *szPMCodeImage, UINT uiPMCodeImageSize, int uiSymbolSize) {

	int		iBaseImagePos;
	int		iValueImagePos;
	int		iBaseWidthLength;
	int		iValueWidthLength;
	int		iBitSize;
	char	szColor	[17];
	char	cTemp;

	iBitSize	= BIT_COUNT_24 / 8;

	iBaseWidthLength = CalcPitch (BIT_COUNT_8, uiSymbolSize);
	iValueWidthLength = CalcPitch (BIT_COUNT_24, uiSymbolSize);

	if (uiQRCodeImageSize < ((unsigned int)iBaseWidthLength * uiSymbolSize)) {
		return RESULT_ERROR_SECURE_MEMORY;	
	}
	if (uiPMCodeImageSize < ((unsigned int)iValueWidthLength * uiSymbolSize)) {
		return RESULT_ERROR_SECURE_MEMORY;	
	}
	memset (szColor, '\0', sizeof (szColor));
	sprintf (szColor, "%0.6X", lColorCode);

	cTemp = szColor [0];
	szColor [0] = szColor [4];
	szColor [4] = cTemp;
	cTemp = szColor [1];
	szColor [1] = szColor [5];
	szColor [5] = cTemp;

	// 重畳処理
	for (int i = 0; i < uiSymbolSize; i ++) {
		for (int j = 0; j < uiSymbolSize; j ++) {
			// ＱＲコード画像の検出位置算出
			iBaseImagePos = (i * iBaseWidthLength) + j;

			// ＰＭコード画像の設定位置算出
			iValueImagePos = (i * iValueWidthLength) + (j * iBitSize);

			if ((szQRCodeImage [iBaseImagePos] & BIT_ON) == BIT_ON) {		// 黒
				szPMCodeImage [iValueImagePos] = szPMCodeImage [iValueImagePos] | h2tb1 (&szColor [0]);
				szPMCodeImage [iValueImagePos + 1] = szPMCodeImage [iValueImagePos + 1] | h2tb1 (&szColor [2]);
				szPMCodeImage [iValueImagePos + 2] = szPMCodeImage [iValueImagePos + 2] | h2tb1 (&szColor [4]);
			}
		}
	}
	return RESULT_OK;
}
