#include "QRCodeEncode.h"
#include "QRCodeMask.h"
#include "QRCodeSymbol.h"
#include "QRCodeDisposition.h"
#include "global.h"
//#include "CtrlBMP.h"
#include "define.h"

// ------------------------------------------------------------------------- //
// 機能概要			:コンストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CQRCodeEncode::CQRCodeEncode () {

	// 初期化
	m_QRCodeImage		= NULL;
	m_uiQRCodeImageSize	= 0;
	m_szEncodeData		= NULL;
	m_uiEncodeDataSize	= 0;
}

// ------------------------------------------------------------------------- //
// 機能概要			:デストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CQRCodeEncode::~CQRCodeEncode () {

	// メモリの開放
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

// ------------------------------------------------------------------------- //
// 機能概要			:QRコード情報のセット									 //
// 引数				:szData				:データ								 //
//					:iDataSize			:データサイズ						 //
//					:iVersion			:型番								 //
//					:iRSLevel			:誤り訂正レベル						 //
//					:iMaskPattern		:マスクパターン						 //
// 戻り値			:RESULT_OK												 //
//					:RESULT_ERROR_SECURE_MEMORY								 //
// 備考				:本来、ＱＲコードの分割データ＆誤り訂正処理はＱＲコード	 //
//					:だが、層の分割があるため、ＰＭコードクラス側で行う		 //
// ------------------------------------------------------------------------- //
int	CQRCodeEncode::SetQRCodeData (char * szData, UINT iDataSize, int iVersion, int iRSLevel, int iMaskPattern) {

	int iSymbolSize;
	int iWidthLength;
	int iImageSize;

	iSymbolSize		= VER2SYM (iVersion);										// 型番に対するシンボルサイズ計算
	iWidthLength	= CalcPitch (BIT_COUNT_8, iSymbolSize);						// 画像横幅サイズの計算
	iImageSize		= iSymbolSize * iWidthLength;								// 画像サイズの計算

	// メモリが既に確保されている場合は、再確保しない（サイズは同一のため）
	if ((int)m_uiQRCodeImageSize != iImageSize + 1) {
		m_uiQRCodeImageSize = iImageSize + 1;
		m_QRCodeImage = (char *)malloc (m_uiQRCodeImageSize);					// 画像データ用メモリの確保
		if (m_QRCodeImage == NULL) {											// メモリ確保失敗
			return RESULT_ERROR_SECURE_MEMORY;
		} 
	}
	memset (m_QRCodeImage, '\0', m_uiQRCodeImageSize);							// 画像メモリの初期化

	m_uiEncodeDataSize = iDataSize;
	m_szEncodeData = (char *)realloc (m_szEncodeData, m_uiEncodeDataSize + 1);	// データ用メモリの確保
	if (m_szEncodeData == NULL) {												// メモリ確保失敗
		return RESULT_ERROR_SECURE_MEMORY;
	} 
	memset (m_szEncodeData, '\0', m_uiEncodeDataSize + 1);						// データバッファの初期化
	memcpy (m_szEncodeData, szData, m_uiEncodeDataSize);						// データの格納

	m_iVersion		= iVersion;													// 型番のセット
	m_iRSLevel		= iRSLevel;													// 誤り訂正レベルのセット
	m_iMaskPattern	= iMaskPattern;												// マスクパターンのセット
	m_iSymbolSize	= iSymbolSize;
	return RESULT_OK;
}

// ------------------------------------------------------------------------- //
// 機能概要			:QRコードのエンコード									 //
// 引数				:なし													 //
// 戻り値			:QRコード画像データサイズ								 //
// 備考				:エンコードできなかった場合は、結果が０となります		 //
//					:単層用に分解・誤り訂正設定処理は呼び出し元にて行います  //
// ------------------------------------------------------------------------- //
UINT CQRCodeEncode::EncodeQRCode () {

	int						iRet;
	int						iMinPenalty = 0;									// 最小失点数
	int						iPenalty	= 0;									// 失点数
	int						iWidthLength;										// 横サイズ
	int						iWordSize;
	CQRCodeSymbol			*pSymbol;											// シンボルクラス
	CQRCodeMask				*pMask;												// マスククラス
	CQRCodeDisposition		*pDisposition;										// データ取得・復号クラス

	SetFunctionModule ();														// 機能モジュールの設定

// データのセット	
	pDisposition = new CQRCodeDisposition;
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
	iRet = pDisposition->SetCodeWordPattern (m_szEncodeData, m_uiEncodeDataSize
								, iWordSize
								, m_iSymbolSize, m_iRSLevel);					// データのセット
//	iRet = pDisposition->SetCodeWordPattern (m_szEncodeData, m_uiEncodeDataSize
//								, QR_VersonInfo[m_iVersion].ncAllCodeWord
//								, m_iSymbolSize, m_iRSLevel);					// データのセット
	delete pDisposition;
	if(RESULT_OK != iRet){
		return 0;
	}
// マスク処理（マスク処理の成否判定ができないため、戻り値やチェックはなし）
	pMask = new CQRCodeMask;
	if (m_iMaskPattern == -1) {													// マスクパターンが自動の場合
		// 最適マスキングパターン選択
		// マスク０～７までマスクを行い、失点数の少ないマスクコードを採用する
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

// 型式/型番の設定
	pSymbol = new CQRCodeSymbol;
	iRet = pSymbol->SetQRCodeSymbol (m_iSymbolSize, m_iRSLevel, m_iMaskPattern);
																				// 型式の取得
	delete pSymbol;
	if(RESULT_OK != iRet){
		return 0;
	}
// ＱＲコード画像を作成する
	iWidthLength = CalcPitch (BIT_COUNT_8, m_iSymbolSize);						// 画像横幅サイズの計算
	for (int i = 0; i < m_iSymbolSize; i ++) {
		for (int j = 0; j < m_iSymbolSize; j ++) {
			// ＢＭＰ画像は４の倍数でないといけないため、シンボルサイズが４の倍数でない場合
			// 穴埋めする必要がある。（あらかじめ初期化を行っているため、処理を飛ばすだけでＯＫ）
// 2006/10/13 tsato update ----------------------------------------------
// ＱＲコード画像を左下から右上に向かって格納する様に変更
			m_QRCodeImage [(i * iWidthLength) + j] = (BYTE)((g_byModuleData[j][m_iSymbolSize - (i + 1)] & 0x11) != 0);
//			m_QRCodeImage [(i * iWidthLength) + j] = (BYTE)((g_byModuleData[j][i] & 0x11) != 0);
// ----------------------------------------------------------------------
		}
	}
	return m_uiQRCodeImageSize;
}

// ------------------------------------------------------------------------- //
// 機能概要			:QRコード画像の取得										 //
// 引数				:szImage			:ＱＲコード画像バッファ				 //
//                  :iBufferSize		:ＱＲコード画像サイズ				 //
// 戻り値			:RESULT_OK												 //
//					:RESULT_ERROR_ENCODE_YET								 //
// 備考				:画像データ＞画像バッファの場合はエラーの方が良いかも	 //
// ------------------------------------------------------------------------- //
int	CQRCodeEncode::GetQRCodeImage (char *szImage, UINT iBufferSize) {

	// ＱＲコード画像あり
	if (m_QRCodeImage != NULL && m_uiQRCodeImageSize != 0) {
		// 画像データがバッファに収まるなら、画像データ分だけセットする
		if (iBufferSize >= m_uiQRCodeImageSize) {
			memcpy (szImage, m_QRCodeImage, m_uiQRCodeImageSize);
		// データバッファサイズがデータより小さい場合は、バッファサイズ分セットする
		} else {
			memcpy (szImage, m_QRCodeImage, iBufferSize);
		}
	} else {
		return RESULT_ERROR_ENCODE_YET;										// 暗号化未完了
	}
	return RESULT_OK;														// 処理完了
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
void CQRCodeEncode::SetFunctionModule () {

	memset (g_byModuleData, '\0', sizeof (g_byModuleData));						// 単層モジュールの初期化

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
		// 　左上横　　　　　　　	左下縦
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

	// ＰＭコード用識別情報　ＱＲコードとしては、機能モジュールとして設定するだけ（上位処理にて設定する）
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

// ------------------------------------------------------------------------- //
// 機能概要			:バージョン(型番)情報パターン配置						 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:機能モジュールの初期化を行い、機能コードの設定をします	 //
// ------------------------------------------------------------------------- //
void CQRCodeEncode::SetVersionPattern() {

	int iVerData;

	// ６型以下はバージョン情報が無いのでそのまま返す
	if (m_iVersion <= 6) {
		return;
	}
	iVerData = m_iVersion << 12;

	// 剰余ビット算出
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

// ------------------------------------------------------------------------- //
// 機能概要			:位置合わせパターンの設定								 //
// 引数				:配置中央Ｘ座標											 //
//					:配置中央Ｙ座標											 //
// 戻り値			:なし													 //
// 備考				:機能モジュールの初期化を行い、機能コードの設定をします	 //
// ------------------------------------------------------------------------- //
void CQRCodeEncode::SetAlignmentPattern(int x, int y) {

	static BYTE byPattern[] = {0x1f,  // 11111b
							   0x11,  // 10001b
							   0x15,  // 10101b
							   0x11,  // 10001b
							   0x1f}; // 11111b

	if (g_byModuleData[x][y] & 0x20) {
		return;																	// 機能モジュールと重複するため除外
	}
	x -= 2; y -= 2;																// 左上隅座標に変換

	for (int i = 0; i < 5; i ++) {
		for (int j = 0; j < 5; j ++) {
			g_byModuleData[x + j][y + i] = (byPattern[i] & (1 << (4 - j))) ? '\x30' : '\x20'; 
		}
	}
}

// ------------------------------------------------------------------------- //
// 機能概要			:失点数の取得											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
int CQRCodeEncode::CountPenalty() {

	int iPenalty = 0;
	int iCount = 0;
	int	k = 0;

	// 同色の列の隣接モジュール
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

	// 同色の行の隣接モジュール
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

	// 同色のモジュールブロック（２×２）
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

	// 同一列における 1:1:3:1:1 比率（暗:明:暗:明:暗）のパターン
	for (int i = 0; i < m_iSymbolSize; i ++) {
		for (int j = 0; j < m_iSymbolSize - 6; j ++) {
			if (((j == 0) ||				 (! (g_byModuleData[i][j - 1] & 0x11))) && // 明 または シンボル外
											 (   g_byModuleData[i][j]     & 0x11)   && // 暗 - 1
											 (! (g_byModuleData[i][j + 1] & 0x11))  && // 明 - 1
											 (   g_byModuleData[i][j + 2] & 0x11)   && // 暗 ┐
											 (   g_byModuleData[i][j + 3] & 0x11)   && // 暗 │3
											 (   g_byModuleData[i][j + 4] & 0x11)   && // 暗 ┘
											 (! (g_byModuleData[i][j + 5] & 0x11))  && // 明 - 1
											 (   g_byModuleData[i][j + 6] & 0x11)   && // 暗 - 1
				((j == m_iSymbolSize - 7) || (! (g_byModuleData[i][j + 7] & 0x11))))   // 明 または シンボル外
			{
				// 前または後に4以上の明パターン
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

	// 同一行における 1:1:3:1:1 比率（暗:明:暗:明:暗）のパターン
	for (int i = 0; i < m_iSymbolSize; i ++) {
		for (int j = 0; j < m_iSymbolSize - 6; j ++) {
			if (((j == 0) ||				 (! (g_byModuleData[j - 1][i] & 0x11))) && // 明 または シンボル外
											 (   g_byModuleData[j]    [i] & 0x11)   && // 暗 - 1
											 (! (g_byModuleData[j + 1][i] & 0x11))  && // 明 - 1
											 (   g_byModuleData[j + 2][i] & 0x11)   && // 暗 ┐
											 (   g_byModuleData[j + 3][i] & 0x11)   && // 暗 │3
											 (   g_byModuleData[j + 4][i] & 0x11)   && // 暗 ┘
											 (! (g_byModuleData[j + 5][i] & 0x11))  && // 明 - 1
											 (   g_byModuleData[j + 6][i] & 0x11)   && // 暗 - 1
				((j == m_iSymbolSize - 7) || (! (g_byModuleData[j + 7][i] & 0x11))))   // 明 または シンボル外
			{
				// 前または後に4以上の明パターン
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

	// 全体に対する暗モジュールの占める割合
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
