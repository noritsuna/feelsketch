#include "QRCodeSymbol.h"
#include "Global.h"
#include "define.h"
//#include "CtrlBMP.h"

// ------------------------------------------------------------------------- //
// 機能概要			:コンストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CQRCodeSymbol::CQRCodeSymbol () {

}

// ------------------------------------------------------------------------- //
// 機能概要			:デストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CQRCodeSymbol::~CQRCodeSymbol () {

}

// ------------------------------------------------------------------------- //
// 機能概要			:シンボル（型式）の取得									 //
// 引数				:szImage			:ＱＲ画像データ						 //
//					:iImageSize			:画像サイズ							 //
//					:iSymbolSize		:シンボルサイズ						 //
//					:iRSLevel			:誤り訂正レベル						 //
//					:iMaskPattern		:マスクパターン						 //
// 戻り値			:RESULT_OK												 //
//					:RESULT_ERROR_QR_CODE_HEADER							 //
// 備考				:画像を保持する必要が無い為、本関数にて解析を行います	 //
// ------------------------------------------------------------------------- //
int CQRCodeSymbol::GetQRCodeSymbol (char * szImage, UINT iImageSize, int iSymbolSize
							  , int *iRSLevel, int *iMaskPattern) {

	int		iModel = 0;
	char	cBit;
	int		i;
	int		iLineLength;

	iLineLength = CalcPitch (BIT_COUNT_8, iSymbolSize);

	for (i = 0; i <= 5; i ++) {
// 2006/10/11 tsato update ----------------------------------------------
		cBit = szImage [(iLineLength * i) + 8];
//		cBit = szImage [iLineLength * (iSymbolSize - (i + 1)) + 8];
// ----------------------------------------------------------------------
		if (cBit == BIT_ON) {												// ビットがＯＮ
			iModel |= (1 << i);
		}
	}
// 2006/10/11 tsato update ----------------------------------------------
	if (BIT_ON == szImage [(iLineLength * 7) + 8]) {			// ビットがＯＮ
//	if (BIT_ON == szImage [iLineLength * (iSymbolSize - 8) + 8]) {			// ビットがＯＮ
// ----------------------------------------------------------------------
		iModel |= (1 << 6);
	}
// 2006/10/11 tsato update ----------------------------------------------
	if (BIT_ON == szImage [(iLineLength * 8) + 8]) {			// ビットがＯＮ
//	if (BIT_ON == szImage [iLineLength * (iSymbolSize - 9) + 8]) {			// ビットがＯＮ
// ----------------------------------------------------------------------
		iModel |= (1 << 7);
	}
// 2006/10/11 tsato update ----------------------------------------------
	if (BIT_ON == szImage [(iLineLength * 8) + 7]) {			// ビットがＯＮ
// ----------------------------------------------------------------------
		iModel |= (1 << 8);
	}

	for (i = 9; i <= 14; i ++) {
// 2006/10/11 tsato update ----------------------------------------------
		cBit = szImage [(iLineLength * 8) + (14 - i)];
// ----------------------------------------------------------------------
		if (cBit == BIT_ON) {												// ビットがＯＮ
			iModel |= (1 << i);
		}
	}

	// マスキング
	iModel ^= 0x5412; // 101010000010010b

	// 誤り訂正（現在未作成）


	// 誤り訂正レベルの取得
	m_iRSLevel		= iModel >> 13;
	switch (m_iRSLevel) {
	case 1:
		*iRSLevel = QR_LEVEL_L;
		break;
	case 0:
		*iRSLevel = QR_LEVEL_M;
		break;
	case 3:
		*iRSLevel = QR_LEVEL_Q;
		break;
	case 2:
		*iRSLevel = QR_LEVEL_H;
		break;

	default: // case QR_LEVEL_H:
		return RESULT_ERROR_QR_CODE_HEADER;

	}

/*
	if (m_iRSLevel != QR_LEVEL_L && m_iRSLevel != QR_LEVEL_M 
		&& m_iRSLevel != QR_LEVEL_Q && m_iRSLevel != QR_LEVEL_H){		// 誤り訂正レベルのチェック（０～３）以外はエラー
		return RESULT_ERROR_QR_CODE_HEADER;
	}
	*iRSLevel = m_iRSLevel;
*/
	// マスクパターンの取得
	m_iMaskPattern	= ((iModel >> 10) & 0x07);
	if (m_iMaskPattern < 0 || m_iMaskPattern > 7) {						// マスクパターンのチェック（０～７）以外はエラー
		return RESULT_ERROR_QR_CODE_HEADER;
	}
	*iMaskPattern = m_iMaskPattern;
	return RESULT_OK;
}

// ------------------------------------------------------------------------- //
// 機能概要			:シンボル（型式）の設定									 //
// 引数				:iSymbolSize		:シンボルサイズ						 //
//					:iRSLevel			:誤り訂正レベル						 //
//					:iMaskPattern		:マスクパターン						 //
// 戻り値			:RESULT_OK												 //
//					:RESULT_ERROR_QR_CODE_HEADER							 //
// 備考				:画像を保持する必要が無い為、本関数にて解析を行います	 //
// ------------------------------------------------------------------------- //
int CQRCodeSymbol::SetQRCodeSymbol (int iSymbolSize, int iRSLevel, int iMaskPattern) {

	int iFormatInfo;

// 型式の設定
	switch (iRSLevel) {
	case QR_LEVEL_L:
		iFormatInfo = 0x08; // 01nnnb
		break;

	case QR_LEVEL_M:
		iFormatInfo = 0x00; // 00nnnb
		break;

	case QR_LEVEL_Q:
		iFormatInfo = 0x18; // 11nnnb
		break;

	case QR_LEVEL_H:
		iFormatInfo = 0x10; // 10nnnb
		break;

	default: // case QR_LEVEL_H:
		;
	}

	iFormatInfo += iMaskPattern;

	int iFormatData = iFormatInfo << 10;

	// 剰余ビット算出
	for (int i = 0; i < 5; ++i)	{
		if (iFormatData & (1 << (14 - i))) {
			iFormatData ^= (0x0537 << (4 - i)); // 10100110111b
		}
	}

	iFormatData += iFormatInfo << 10;

	// マスキング
	iFormatData ^= 0x5412; // 101010000010010b

	// 左上位置検出パターン周り配置
	for (int i = 0; i <= 5; ++i) {
		g_byModuleData[8][i] = (iFormatData & (1 << i)) ? '\x30' : '\x20';
	}
	g_byModuleData[8][7] = (iFormatData & (1 << 6)) ? '\x30' : '\x20';
	g_byModuleData[8][8] = (iFormatData & (1 << 7)) ? '\x30' : '\x20';
	g_byModuleData[7][8] = (iFormatData & (1 << 8)) ? '\x30' : '\x20';

	for (int i = 9; i <= 14; ++i) {
		g_byModuleData[14 - i][8] = (iFormatData & (1 << i)) ? '\x30' : '\x20';
	}

	// 右上位置検出パターン下配置
	for (int i = 0; i <= 7; ++i) {
		g_byModuleData[iSymbolSize - 1 - i][8] = (iFormatData & (1 << i)) ? '\x30' : '\x20';
	}

	// 左下位置検出パターン右配置
	g_byModuleData[8][iSymbolSize - 8] = '\x30'; // 固定暗モジュール

	for (int i = 8; i <= 14; ++i) {
		g_byModuleData[8][iSymbolSize - 15 + i] = (iFormatData & (1 << i)) ? '\x30' : '\x20';
	}
	return RESULT_OK;
}
