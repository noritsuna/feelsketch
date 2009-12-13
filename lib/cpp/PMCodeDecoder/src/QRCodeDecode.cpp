#include "QRCodeDecode.h"
#include "QRCodeMask.h"
#include "QRCodeSymbol.h"
#include "QRCodeRestoration.h"
#include "global.h"
#include "define.h"


// ------------------------------------------------------------------------- //
// 機能概要			:コンストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CQRCodeDecode::CQRCodeDecode () {

	// 初期化
	m_QRCodeImage		= NULL;
	m_uiQRCodeImageSize	= 0;
	m_szDecodeData		= NULL;
	m_uiDecodeDataSize	= 0;
}

// ------------------------------------------------------------------------- //
// 機能概要			:デストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CQRCodeDecode::~CQRCodeDecode () {

	// メモリの開放
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

// ------------------------------------------------------------------------- //
// 機能概要			:QRコードのセット										 //
// 引数				:szImage			:ＱＲ画像データ						 //
//					:iImageSize			:画像サイズ							 //
//					:iVersion			:型番								 //
// 戻り値			:RESULT_OK												 //
//					:RESULT_ERROR_SECURE_MEMORY								 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
int	CQRCodeDecode::SetQRCodeImage (char * szImage, UINT iImageSize, int iVersion) {

	// メモリが既に確保されている場合は、再確保しない（サイズは同一のため）
	if (m_QRCodeImage == NULL) {
		m_QRCodeImage = (char *)malloc (iImageSize + 1);						// 画像データ用メモリの確保
		if (m_QRCodeImage == NULL) {											// メモリ確保失敗
			return RESULT_ERROR_SECURE_MEMORY;
		} 
	}		

	memset (m_QRCodeImage, '\0', iImageSize + 1);
	memcpy (m_QRCodeImage, szImage, iImageSize);							// 画像データのセット
	m_uiQRCodeImageSize = iImageSize;										// 画像データサイズのセット
	m_iVersion			= iVersion;											// 型番のセット
	m_iSymbolSize		= VER2SYM (m_iVersion);								// シンボルサイズのセット
	return RESULT_OK;
}

// ------------------------------------------------------------------------- //
// 機能概要			:QRコードのデコード										 //
// 引数				:なし													 //
// 戻り値			:復号データサイズ										 //
// 備考				:復号できなかった場合は、結果が０となります				 //
// ------------------------------------------------------------------------- //
UINT CQRCodeDecode::DecodeQRCodeImage () {

	int						iRet;
	int						iWordSize;
	CQRCodeSymbol			*pSymbol;										// シンボル解析クラス
	CQRCodeMask				*pMask;											// マスククラス
	CQRCodeRestoration		*pRestoration;									// データ取得・復号クラス

	SetFunctionModule ();													// 機能モジュールの設定

// 型式の取得
	pSymbol = new CQRCodeSymbol;
	iRet = pSymbol->GetQRCodeSymbol (m_QRCodeImage, m_uiQRCodeImageSize, m_iSymbolSize
								, &m_iRSLevel, &m_iMaskPattern);			// 型式の取得
	delete pSymbol;
	if(RESULT_OK != iRet){
		return 0;
	}

// マスク解除処理（マスク処理の成否判定ができないため、戻り値やチェックはなし）
	pMask = new CQRCodeMask;
	pMask->GetMaskQRCode (m_QRCodeImage, m_uiQRCodeImageSize, m_iMaskPattern, m_iVersion);
	delete pMask;

// データの取得処理
	pRestoration = new CQRCodeRestoration;

	// 2006/11/27 色識別コード拡張
// 2006/12/27 誤り訂正に対応する為に、テーブル数を変更
	iWordSize = QR_VersonInfo[m_iVersion].ncAllCodeWord;
/*
	if (m_iVersion < 3) {
		iWordSize = QR_VersonInfo[m_iVersion].ncAllCodeWord;
	} else {
		iWordSize = QR_VersonInfo[m_iVersion].ncAllCodeWord - 6;
	}
*/
	iRet = pRestoration->SetCodeWordPattern (m_QRCodeImage, m_uiQRCodeImageSize
											, iWordSize, m_iSymbolSize, m_iRSLevel);
//	iRet = pRestoration->SetCodeWordPattern (m_QRCodeImage, m_uiQRCodeImageSize
//											, QR_VersonInfo[m_iVersion].ncAllCodeWord, m_iSymbolSize, m_iRSLevel);

	if(RESULT_OK != iRet){
		delete pRestoration;
		return 0;
	}
	// データ用格納領域の確保
	// 以前のデータがある場合は、開放する
	if (m_szDecodeData != NULL) {
		free (m_szDecodeData);
		m_szDecodeData = NULL;
		m_uiDecodeDataSize = 0;
	}
	m_uiDecodeDataSize = iWordSize;											// 型に対するデータサイズの取得
	m_szDecodeData = (char *)malloc (m_uiDecodeDataSize + 1);
	if (m_szDecodeData == NULL) {
		delete pRestoration;
		return 0;
	}
	memset (m_szDecodeData, '\0', m_uiDecodeDataSize + 1);
	// データの取得
	iRet = pRestoration->GetCodeData (m_szDecodeData, m_uiDecodeDataSize);
	if(RESULT_OK != iRet){
		delete pRestoration;
		return 0;
	}
	delete pRestoration;
	return m_uiDecodeDataSize;												// 復号データサイズを返す
}

// ------------------------------------------------------------------------- //
// 機能概要			:復号データの取得										 //
// 引数				:szData			:復号したデータバッファ					 //
//                  :iBufferSize	:復号データサイズ						 //
// 戻り値			:RESULT_OK												 //
//					:RESULT_ERROR_DECODE_YET								 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
int	CQRCodeDecode::GetDecodeData (char *szData, UINT iBufferSize) {

	// 復号したデータあり
	if (m_szDecodeData != NULL && m_uiDecodeDataSize != 0) {
		// 復号データがバッファに収まるなら、復号データ分だけセットする
		if (iBufferSize >= m_uiDecodeDataSize) {
			memcpy (szData, m_szDecodeData, m_uiDecodeDataSize);
		// データバッファサイズがデータより小さい場合は、バッファサイズ分セットする
		} else {
			memcpy (szData, m_szDecodeData, iBufferSize);
		}
	} else {
		return RESULT_ERROR_DECODE_YET;										// 復号未完了
	}
	return RESULT_OK;														// 処理完了
}

// ------------------------------------------------------------------------- //
// 機能概要			:復号データの取得										 //
// 引数				:iRSLevel		:誤り訂正レベル							 //
// 戻り値			:RESULT_OK												 //
//					:RESULT_ERROR_DECODE_YET								 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
int	CQRCodeDecode::GetQRCodeStatus (int * iRSLevel) {

	if (m_szDecodeData != NULL && m_uiDecodeDataSize != 0) {
		*iRSLevel		= m_iRSLevel;
	} else {
		return RESULT_ERROR_DECODE_YET;										// 復号未完了
	}
	return RESULT_OK;														// 処理完了
}

// ------------------------------------------------------------------------- //
// 機能概要			:マスク番号の取得										 //
// 引数				:なし													 //
// 戻り値			:マスク番号												 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
int	CQRCodeDecode::GetMaskNumber () {
	return m_iMaskPattern;
}


// ------------------------------------------------------------------------- //
// --------------------------- 以下private 関数 ---------------------------- //
// ------------------------------------------------------------------------- //

// ------------------------------------------------------------------------- //
// 機能概要			:機能コードの設定										 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:機能モジュールの初期化を行い、機能コードの設定をします	 //
//					:本関数の処理はＱＲコード作成ツールを参考にしています	 //
// ------------------------------------------------------------------------- //
void CQRCodeDecode::SetFunctionModule () {

	memset (g_byModuleData, '\0', sizeof (g_byModuleData));					// 単層モジュールの初期化

	// 位置検出パターンの設定
	SetFinderPattern(0, 0);
	SetFinderPattern(m_iSymbolSize - 7, 0);
	SetFinderPattern(0, m_iSymbolSize - 7);

	// 位置検出パターンセパレータ
	for (int i = 0; i < 8; i ++) {
		g_byModuleData[i][7]					= g_byModuleData[7][i] = '\x20';
		g_byModuleData[m_iSymbolSize - 8][i]	= g_byModuleData[m_iSymbolSize - 8 + i][7] = '\x20';
		g_byModuleData[i][m_iSymbolSize - 8]	= g_byModuleData[7][m_iSymbolSize - 8 + i] = '\x20';
	}

	// フォーマット情報記述位置を機能モジュール部として登録
	for (int i = 0; i < 9; i ++) {
		// 　左上横　　　　　　　　　					左下縦
		g_byModuleData[i][8] = g_byModuleData[8][i] = '\x20';
	}

	for (int i = 0; i < 8; i ++) {
		//		右上横　　　　　　　　　　　　　　　　　　　　　左下縦
		g_byModuleData[m_iSymbolSize - 8 + i][8] = g_byModuleData[8][m_iSymbolSize - 8 + i] = '\x20';
	}

// -----------------------------------------------------------------------------------------
// 2007/01/05 型番コードを使用しない様対応
	// バージョン情報パターン
//	SetVersionPattern();
// -----------------------------------------------------------------------------------------

	// 位置合わせパターン
	for (int i = 0; i < QR_VersonInfo[m_iVersion].ncAlignPoint; i ++) {
		SetAlignmentPattern(QR_VersonInfo[m_iVersion].nAlignPoint[i], 6);		// 右上
		SetAlignmentPattern(6, QR_VersonInfo[m_iVersion].nAlignPoint[i]);		// 左下

		for (int j = 0; j < QR_VersonInfo[m_iVersion].ncAlignPoint; j ++) {
			SetAlignmentPattern(QR_VersonInfo[m_iVersion].nAlignPoint[i]
				, QR_VersonInfo[m_iVersion].nAlignPoint[j]);
		}
	}

	// タイミングパターン
	for (int i = 8; i <= m_iSymbolSize - 9; i ++) {
		g_byModuleData[i][6] = (i % 2) == 0 ? '\x30' : '\x20';
		g_byModuleData[6][i] = (i % 2) == 0 ? '\x30' : '\x20';
	}

	// ＰＭコード用識別情報　ＱＲコードとしては、機能モジュールとして設定するだけ
#if 0
	for (int i = 0; i <= 5; i ++) {
		for (int j = 9; j <= 12; j ++) {
			g_byModuleData[i][j] = '\x20';
		}
	}
#else
	// 2006/11/27 色識別コードのサイズを層数により変化させる
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

// ------------------------------------------------------------------------- //
// 機能概要			:機能コードの設定										 //
// 引数				:左下隅Ｘ座標											 //
//					:左下隅Ｙ座標											 //
// 戻り値			:なし													 //
// 備考				:機能モジュールの初期化を行い、機能コードの設定をします	 //
// ------------------------------------------------------------------------- //
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

// ------------------------------------------------------------------------- //
// 機能概要			:バージョン(型番)情報パターン配置						 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:機能モジュールの初期化を行い、機能コードの設定をします	 //
// ------------------------------------------------------------------------- //
void CQRCodeDecode::SetVersionPattern() {

	// ６型以下はバージョン情報が無いのでそのまま返す
	if (m_iVersion <= 6) {
		return;
	}
	int nVerData = m_iVersion << 12;

	// 剰余ビット算出
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

// ------------------------------------------------------------------------- //
// 機能概要			:位置合わせパターンの設定								 //
// 引数				:配置中央Ｘ座標											 //
//					:配置中央Ｙ座標											 //
// 戻り値			:なし													 //
// 備考				:機能モジュールの初期化を行い、機能コードの設定をします	 //
// ------------------------------------------------------------------------- //
void CQRCodeDecode::SetAlignmentPattern(int x, int y) {

	static BYTE byPattern[] = {0x1f,  // 11111b
							   0x11,  // 10001b
							   0x15,  // 10101b
							   0x11,  // 10001b
							   0x1f}; // 11111b

	if (g_byModuleData[x][y] & 0x20) {
		return; // 機能モジュールと重複するため除外
	}
	x -= 2; y -= 2; // 左上隅座標に変換

	for (int i = 0; i < 5; ++i)	{
		for (int j = 0; j < 5; ++j)	{
			g_byModuleData[x + j][y + i] = (byPattern[i] & (1 << (4 - j))) ? '\x30' : '\x20'; 
		}
	}
}

