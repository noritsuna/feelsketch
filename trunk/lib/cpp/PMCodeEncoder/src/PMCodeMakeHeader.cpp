#include "define.h"
#include "PMCodeMakeHeader.h"
#include "global.h"
//#include "CtrlBMP.h"
#include <stdio.h>
// ------------------------------------------------------------------------- //
// 本関数に渡されるＰＭコード画像は補正後の画像とします。
// 
// ------------------------------------------------------------------------- //
#define QR_CODE_BIT_ON		'\x00'				// ビットＯＮ（黒）
#define QR_CODE_BIT_OFF		'\xFF'				// ビットＯＦＦ（白）

// ------------------------------------------------------------------------- //
// 機能概要			:コンストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CPMCodeMakeHeader::CPMCodeMakeHeader () {

}

// ------------------------------------------------------------------------- //
// 機能概要			:デストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CPMCodeMakeHeader::~CPMCodeMakeHeader () {

}

// ------------------------------------------------------------------------- //
// 機能概要			:ＰＭコード画像のセット									 //
// 引数				:szImage		:ＰＭコード画像データ					 //
//                  :iImageSize		:ＰＭコード画像データサイズ				 //
//                  :iSymbolSize	:ＰＭコード画像シンボルサイズ			 //
// 戻り値			:RESULT_OK												 //
//                  :RESULT_ERROR_SECURE_MEMORY								 //
// 備考				:画像を保持する必要が無いので、本関数で処理を行います	 //
// ------------------------------------------------------------------------- //
int CPMCodeMakeHeader::SetPMCodeHeader (char *szImage, UINT iImageSize, int iSymbolSize, int iLayer) {

	int		iBitSize;
	int		iLineSize;
	int		iX, iY;
	int		iDataPos;
	long	lColorCode;
	char	szColor [8];
	char	szColorHex [4];
	char	cTemp;

	m_iLayer		= iLayer;
	m_iSymbolSize	= iSymbolSize;
	m_iVersion		= SYM2VER (m_iSymbolSize);	

// 層数の設定
	iBitSize	= BIT_COUNT_24 / 8;
	iLineSize	= (int)CalcPitch (BIT_COUNT_24, m_iSymbolSize);

	// データのセット
	for (int i = 0; i < LAYER_BIT; i ++) {
		iX = (LAYER_CODE_POS_X * iBitSize);
// 2006/10/12 tsato update ----------------------------------------------
//		iY = 9 + i;
		iY = m_iSymbolSize - (10 + i);
// ----------------------------------------------------------------------
																			// 該当位置座標の計算
		iDataPos = (iY * iLineSize) + iX;
		memset (szColor, '\0', sizeof (szColor));
		memcpy (szColor, &g_szLayerSetting [m_iLayer - MIN_LAYERSIZE][i * 3], PM_CODE_BIT_SIZE);

// comment out start 2009/11/18 gabu
// Windows版では、以下の3行でRとBを入れ替えて、うまくいっているが、
// OSS版（開発はMac OS X）では、なぜかRとBが反転する。
// 以下の3行をコメントアウトして、RとBの入れ替えをやめると、デコードが成功する。
// （以下の3行で、RとBを入れ替えると、デコード時にエラーになる。）
//		cTemp = szColor [0];
//		szColor [0] = szColor [2];
//		szColor [2] = cTemp;
// comment out end 2009/11/18 gabu
		memcpy (&szImage [iDataPos], szColor, PM_CODE_BIT_SIZE);
	}
// 色コードの設定
#if 0
	for (int i = 0; i < PM_CODE_HEADER_COLOR_CODE; i ++) {
		iX = (COLOR_CODE_POS_X - (i / 4)) * iBitSize;
// 2006/10/12 tsato update ----------------------------------------------
//		iY = 9 + (i % 4);
		iY = m_iSymbolSize - (10 + (i % 4));
// ----------------------------------------------------------------------
		// 該当位置座標の計算
		iDataPos = (iY * iLineSize) + iX;
		lColorCode = 0;

		 // 色コード設定以上の項目は設定しない
		if (g_ColorCodeSetting == 0) {										// 定義ファイル
			lColorCode = g_ColorCodeSettingDefinition [i];	
		} else {															// デフォルト値
			if (m_iLayer == 3) {											// 層数は３
				if (i < m_iLayer) {
					lColorCode = g_ColorCodeSetting3 [i];
				} else {
					break;													// それ以上は設定しない
				}
			} else if (m_iLayer > 9) {										// 層数は１０～２４
				if (i < m_iLayer || i < PM_CODE_HEADER_COLOR_CODE) {
					lColorCode = g_ColorCodeSetting10to24 [i];	
				} else {
					break;													// それ以上は設定しない
				}
			} else {														// 層数は３～９
				if (i < m_iLayer) {
					lColorCode = g_ColorCodeSetting4to9 [i];	
				} else {
					break;													// それ以上は設定しない
				}
			}
		}
		memset (szColor, '\0', sizeof (szColor));
		sprintf (szColor, "%0.6X", lColorCode);

		cTemp = szColor [0];
		szColor [0] = szColor [4];
		szColor [4] = cTemp;
		cTemp = szColor [1];
		szColor [1] = szColor [5];
		szColor [5] = cTemp;
		
		for (int j = 0; j < PM_CODE_BIT_SIZE; j ++) {
			szColorHex [j] = h2tb1 (&szColor [j * 2]);
		}
		memcpy (&szImage [iDataPos], szColorHex, PM_CODE_BIT_SIZE);
	}
#else 
// 2006/11/28 色識別コード拡張-----------------------------------------------
	for (int i = 0; i < g_ColorCodeTableSize; i ++) {
		if (i < PM_CODE_HEADER_COLOR_CODE) {
			iX = (COLOR_CODE_POS_X - (i / 4)) * iBitSize;
			iY = m_iSymbolSize - (10 + (i % 4));
		} else if (i >= PM_CODE_HEADER_COLOR_CODE && i < PM_CODE_HEADER_COLOR_CODE2){
			iX = (LAYER_CODE_POS_X - ((i - PM_CODE_HEADER_COLOR_CODE) / 4)) * iBitSize;
			iY = m_iSymbolSize - (14 + ((i - PM_CODE_HEADER_COLOR_CODE) % 4));
		} else {
			iX = (LAYER_CODE_POS_X - ((i - PM_CODE_HEADER_COLOR_CODE2) / 4)) * iBitSize;
			iY = m_iSymbolSize - (18 + ((i - PM_CODE_HEADER_COLOR_CODE2) % 4));
		}

		// 該当位置座標の計算
		iDataPos = (iY * iLineSize) + iX;
		lColorCode = 0;
		lColorCode = g_ColorCodeTable [i];
		memset (szColor, '\0', sizeof (szColor));
		sprintf (szColor, "%0.6X", lColorCode);

		cTemp = szColor [0];
		szColor [0] = szColor [4];
		szColor [4] = cTemp;
		cTemp = szColor [1];
		szColor [1] = szColor [5];
		szColor [5] = cTemp;
		
		for (int j = 0; j < PM_CODE_BIT_SIZE; j ++) {
			szColorHex [j] = h2tb1 (&szColor [j * 2]);
		}
		memcpy (&szImage [iDataPos], szColorHex, PM_CODE_BIT_SIZE);
// --------------------------------------------------------------------------
	}
#endif
	return RESULT_OK;
}
