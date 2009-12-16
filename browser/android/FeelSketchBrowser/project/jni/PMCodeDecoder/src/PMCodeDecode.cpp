#include "PMCodeDecode.h"
#include "QRCodeDecode.h"
#include "global.h"
#include "define.h"
#include "ReedSolomon.h"
#include "mathematics.h"


// ------------------------------------------------------------------------- //
// 機能概要			:コンストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CPMCodeDecode::CPMCodeDecode () {

	// 初期化
	m_szBaseImage		= NULL;
	m_uiBaseImageSize	= 0;
	m_szDecodeData		= NULL;
	m_uiDecodeDataSize	= 0;
//	m_szCorrectImage	= NULL;
//	m_uiCorrectImageSize= 0;
	m_Width				= 0;
	m_Height			= 0;
	m_szQRCodeImage		= NULL;
	m_uiQRCodeImageSize	= 0;
}

// ------------------------------------------------------------------------- //
// 機能概要			:デストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CPMCodeDecode::~CPMCodeDecode () {

	// メモリの開放
	if (m_szBaseImage != NULL) {											// 元画像バッファの開放
		free (m_szBaseImage);
		m_szBaseImage = NULL;
		m_uiBaseImageSize = 0;
	}
	if (m_szDecodeData != NULL) {											// 復号データバッファの開放
		free (m_szDecodeData);
		m_szDecodeData = NULL;
		m_uiDecodeDataSize = 0;
	}
//	if (m_szCorrectImage != NULL) {											// 補正画像バッファの開放
//		free (m_szCorrectImage);
//		m_uiCorrectImageSize = 0;
//	}
	if (m_szQRCodeImage != NULL){
		free (m_szQRCodeImage);
		m_szQRCodeImage = NULL;
	}
}

// 基準色のセット
void CPMCodeDecode::SetBaseColor (int *baseColors, int layer) {
	int i;
	for (i = 0; i < layer; i++) {
		g_ColorCodeSettingDefinition[i] = baseColors[i];
	}
}

// ------------------------------------------------------------------------- //
// 機能概要			:元画像のセット											 //
// 引数				:szImage		:ＰＭ画像データ							 //
//                  :iImageSize		:ＰＭ画像データサイズ					 //
// 戻り値			:RESULT_OK												 //
//                  :RESULT_ERROR_SECURE_MEMORY								 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
int CPMCodeDecode::SetBaseImage (char *szImage, UINT uiImageSize, int iWidth, int iHeight) {

	if (m_szBaseImage != NULL) {											// 既に読み込まれている場合は、一度メモリを開放する
		free (m_szBaseImage);
		m_szBaseImage = NULL;
	}
	m_szBaseImage = (char *)realloc (m_szBaseImage, uiImageSize);			// 画像データ用メモリの確保
	if (m_szBaseImage == NULL) {											// メモリ確保失敗
		return RESULT_ERROR_SECURE_MEMORY;
	} 
	memset (m_szBaseImage, '\0', uiImageSize);								// 画像メモリの初期化
	memcpy (m_szBaseImage, szImage, uiImageSize);							// 画像データのセット
	m_uiBaseImageSize = uiImageSize;										// 画像データサイズのセット
	m_Width		= iWidth;													// 幅のセット
	m_Height	= iHeight;													// 高さのセット
	return RESULT_OK;
}

// ------------------------------------------------------------------------- //
// 機能概要			:復号処理												 //
// 引数				:なし													 //
// 戻り値			:復号データサイズ										 //
// 備考				:復号できなかった場合は、結果が以下となります			 //
// ------------------------------------------------------------------------- //
int CPMCodeDecode::DecodePMCode () {

	CQRCodeDecode	*pQRCodeDecode						= NULL;
	UINT			uiDecodeDataSize;
	char			*szQRCodeImage = NULL;
	int				iQRCodeImageSize;
	int				iQRCodeLengthSize;
	long			lColorCode;
	int				iRet;
	CReedSolomon	cReedSolomon;
	BOOL			bRet;
	int				iDataSize;
	int				iBlockSize;
	int				iCorrectPos = 0;
	int				iCorrectDataSize;
	int				iRSBlock1;
	int				iRSBlock2;
	BYTE			szCorrectBuf [256];
	BYTE			szWork [256];
	char			*szDecodeData = NULL;

	// メモリが既に確保されていれば、一旦開放する
	if (m_szDecodeData != NULL) {
		free (m_szDecodeData);
		m_szDecodeData = NULL;
		m_uiDecodeDataSize = 0;
	}
	// 画像未読込
	if (m_szBaseImage == NULL || m_uiBaseImageSize == 0) {
		return RESULT_ERROR_IMAGEFILE_READ_YET;
	}

	m_iVersion		= SYM2VER(m_Width);								// 型番の決定
	m_iSymbolSize	= m_Width;										// シンボルサイズのセット

// カラーコードの取得
	iRet = GetColorCode (g_PMCodeColor, m_szBaseImage, m_uiBaseImageSize, m_iSymbolSize, m_iVersion);
	if(RESULT_OK != iRet){
		return iRet;
	}

// 層数の取得
	m_iLayer = GetLayerCount (m_szBaseImage, m_uiBaseImageSize, m_iSymbolSize);

	if(0 == m_iLayer){														// 層数が０の場合は解析失敗とする
		return RESULT_ERROR_ANALYSIS_LAYER;
	}

	// ３層１型or２型		：色識別コードは通常
	// ３層３型以降or６層	：色識別コードは拡張
	// ６層については、画面にて１，２型は作成出来ない様にしている
	if (m_iVersion < 3) {
		MakeColorTable (3, m_iLayer);
	} else {
		MakeColorTable (6, m_iLayer);
	}

// 単層への分離処理とＱＲコード解析
	iQRCodeLengthSize	= CalcPitch (BIT_COUNT_8, m_iSymbolSize);
	iQRCodeImageSize	= iQRCodeLengthSize * m_iSymbolSize;
																			// ＱＲデータ用メモリの確保
	if (szQRCodeImage != NULL){
		free (szQRCodeImage);
		szQRCodeImage = NULL;
	}
	szQRCodeImage = (char *)malloc (iQRCodeImageSize + 1);					// 解析用データバッファの確保
	if (szQRCodeImage == NULL) {											// メモリ確保失敗
		return RESULT_ERROR_SECURE_MEMORY;
	}
	memset (szQRCodeImage, '\0', iQRCodeImageSize + 1);				
	pQRCodeDecode = new CQRCodeDecode;

																			// 解析データバッファの確保
	m_szDecodeData = (char *)realloc (m_szDecodeData, (QR_VersonInfo[m_iVersion].ncAllCodeWord * m_iLayer) + 1);
	memset (m_szDecodeData, '\0', (QR_VersonInfo[m_iVersion].ncAllCodeWord * m_iLayer) + 1);

	for (int i = 0; i < m_iLayer; i ++ ) {									// 層数分だけデータ取得を行う
		// 単層への分離処理
#if PM_CODE_COLOR
		// 本来はＰＭコード中に設定されている色コードを使用するが、本アプリでは、色設定画面にて
		// 設定及び、使用する色設定によりカラーコードを決定する
		if (g_ColorCodeSetting == 0) {										// 定義ファイル
			lColorCode = g_ColorCodeSettingDefinition [i];	
		} else {															// デフォルト値
			if (m_iLayer == 3) {											// 層数は３
				lColorCode = g_ColorCodeSetting3 [i];	
			} else if (m_iLayer >9) {										// 層数は１０〜２４
				lColorCode = g_ColorCodeSetting10to24 [i];	
			} else {														// 層数は３〜９
				lColorCode = g_ColorCodeSetting4to9 [i];	
			}
		}
#else
		lColorCode = g_PMCodeColor [i];										// PMコード設定のカラーコード
#endif
		if (lColorCode == 0) {												// カラーコード設定がされていない場合はエラーとする
			delete pQRCodeDecode;
			pQRCodeDecode = NULL;
			free (szQRCodeImage);
			szQRCodeImage = NULL;
			return RESULT_ERROR_NON_SUPORT_COLOR;
		}
		iRet = PMCodeToQRCode (m_szBaseImage, m_uiBaseImageSize, lColorCode
										, szQRCodeImage, iQRCodeImageSize, m_iSymbolSize);
		if(RESULT_OK != iRet){
			free (szQRCodeImage);
			szQRCodeImage = NULL;
			return RESLUT_ERROR_LAYER_RESOLUTION;
		}
// ＱＲコード解析
																			// ＱＲコード画像の設定
		iRet = pQRCodeDecode->SetQRCodeImage (szQRCodeImage, iQRCodeImageSize, m_iVersion);
		if(RESULT_OK != iRet){
			free (szQRCodeImage);
			szQRCodeImage = NULL;
			delete pQRCodeDecode;
			pQRCodeDecode = NULL;
			return RESULT_ERROR_SECURE_MEMORY;
		}

		uiDecodeDataSize = pQRCodeDecode->DecodeQRCodeImage ();				// ＱＲコード解析
		if(0 == uiDecodeDataSize){
			free (szQRCodeImage);
			szQRCodeImage = NULL;
			delete pQRCodeDecode;
			pQRCodeDecode = NULL;
			return RESULT_ERROR_ANALYSIS_QR_IMAGE;
		}

		m_iMaskPattern [i] = pQRCodeDecode->GetMaskNumber ();
// 2006/12/26 誤り訂正に対応 -----------------------------------------------------------------------------------
		iRet = pQRCodeDecode->GetQRCodeStatus (&m_iRSLevel);
		if(RESULT_OK != iRet){
			free (szQRCodeImage);
			szQRCodeImage = NULL;
			delete pQRCodeDecode;
			pQRCodeDecode = NULL;
			return RESULT_ERROR_ANALYSIS_QR_STATUS;
		}
		// 解析データの取得
		if (szDecodeData != NULL) {
			free (szDecodeData);
			szDecodeData = NULL;
		}
		szDecodeData = (char *)malloc (uiDecodeDataSize + 1);
		memset (szDecodeData, '\0', uiDecodeDataSize + 1);
		iRet = pQRCodeDecode->GetDecodeData (szDecodeData, uiDecodeDataSize);

		if(RESULT_OK != iRet){
			free (szQRCodeImage);
			szQRCodeImage = NULL;
			delete pQRCodeDecode;
			pQRCodeDecode = NULL;
			return RESULT_ERROR_ANALYSIS_QR_DECODE;
		}
		// 誤り訂正処理

		iDataSize = uiDecodeDataSize;										// 解析データサイズをセット
		iCorrectPos = 0;
		iRSBlock1 = QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncRSBlock;
		iRSBlock2 = QR_VersonInfo[m_iVersion].RS_BlockInfo2 [m_iRSLevel].ncRSBlock;

		for (int j = 0; j < (iRSBlock1 + iRSBlock2); j ++) {
			// jをブロック位置としてＲＳブロック情報１or２のどちらを使用するか判断する
			if (QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncRSBlock > j){
				iBlockSize = QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncAllCodeWord;
				cReedSolomon.SetCorrectCodeSize (iBlockSize	- QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncDataCodeWord);
			} else {
				iBlockSize = QR_VersonInfo[m_iVersion].RS_BlockInfo2 [m_iRSLevel].ncAllCodeWord;
				cReedSolomon.SetCorrectCodeSize (iBlockSize - QR_VersonInfo[m_iVersion].RS_BlockInfo2 [m_iRSLevel].ncDataCodeWord);
			}
			memset (szWork, NULL, sizeof (szWork));
			memset (szCorrectBuf, NULL, sizeof (szCorrectBuf));

			// 各ブロックでのデコード
			if (iDataSize < iBlockSize) {									// ブロックサイズより、データ数が少ないとき
				memcpy (szWork, &szDecodeData [iCorrectPos], iDataSize);
				bRet = cReedSolomon.Decode (szWork, iBlockSize, szCorrectBuf, &iCorrectDataSize);
				if(bRet != TRUE){
					free (szQRCodeImage);
					szQRCodeImage = NULL;
					delete pQRCodeDecode;
					pQRCodeDecode = NULL;
					free (szDecodeData);
					szDecodeData = NULL;
					return RESULT_ERROR_ANALYSIS_QR_BIG_BLOCK_DECODE;
				}
			} else {														// ブロックサイズより、データ数が多いとき
				memcpy (szWork, &szDecodeData [iCorrectPos], iBlockSize);
				bRet = cReedSolomon.Decode (szWork, iBlockSize, szCorrectBuf, &iCorrectDataSize);
				if(bRet != TRUE){
					free (szQRCodeImage);
					szQRCodeImage = NULL;
					delete pQRCodeDecode;
					pQRCodeDecode = NULL;
					free (szDecodeData);
					szDecodeData = NULL;
					return RESULT_ERROR_ANALYSIS_QR_SMALL_BLOCK_DECODE;
				}
			}
			// エンコードしたデータを並べる
			memcpy (&m_szDecodeData [m_uiDecodeDataSize], szCorrectBuf, iCorrectDataSize);
			m_uiDecodeDataSize += iCorrectDataSize;								// データ数の設定
			iCorrectPos += iBlockSize;											// ブロックサイズのセット
			iDataSize -= iBlockSize;
		}
		if (szDecodeData != NULL) {
			free (szDecodeData);
			szDecodeData = NULL;
		}
	// -------------------------------------------------------------------------------------------------------------
	}
	delete pQRCodeDecode;
	pQRCodeDecode = NULL;

	if (szQRCodeImage != NULL) {											// メモリの開放
		free (szQRCodeImage);
		szQRCodeImage = NULL;
	}
	return m_uiDecodeDataSize;
}

// ------------------------------------------------------------------------- //
// 機能概要			:復号したデータの取得									 //
// 引数				:szData			:復号したデータバッファ					 //
//                  :iBufferSize	:復号データサイズ						 //
// 戻り値			:RESULT_OK												 //
//					:RESULT_ERROR_DECODE_YET								 //
// 備考																		 //
// ------------------------------------------------------------------------- //
int CPMCodeDecode::GetPMCodeDecodeData (char *szData, UINT uiBufferSize) {

	// 復号したデータあり
	if (m_szDecodeData != NULL && m_uiDecodeDataSize != 0) {
		if (m_uiDecodeDataSize >= uiBufferSize) {							// バッファサイズが小さい場合は入る分だけ入れる
			memcpy (szData, m_szDecodeData, uiBufferSize);
		} else {
			memcpy (szData, m_szDecodeData, m_uiDecodeDataSize);
		}
	} else {
		return RESULT_ERROR_DECODE_YET;										// 復号未完了
	}
	return RESULT_OK;														// 処理完了
}

// ------------------------------------------------------------------------- //
// 機能概要			:ＱＲコード画像サイズの取得								 //
// 引数				:iLayer			:サイズ取得を行う層番号					 //
// 戻り値			:ＱＲコード画像サイズ									 //
// 備考				:ＱＲコード画像が無い場合は、結果が０となります			 //
// ------------------------------------------------------------------------- //
UINT CPMCodeDecode::GetQRCodeSize (int iLayer) {

	// 補正画像あり
	//if (m_szCorrectImage != NULL && m_uiCorrectImageSize != 0){
		return m_Width * m_Height;
	//}
	//return 0;
}

// ------------------------------------------------------------------------- //
// 機能概要			:ＱＲコード画像の取得									 //
// 引数				:szImage		:ＱＲコード画像							 //
//                  :iBufferSize	:ＱＲコードバッファサイズ				 //
//                  :iLayer			:層番号									 //
// 戻り値			:RESULT_OK												 //
//					:RESULT_ERROR_DECODE_YET								 //
//					:RESULT_ERROR_NON_SUPORT_COLOR							 //
// 備考				::色識別コードは既に読込まれている状態であること		 //
// ------------------------------------------------------------------------- //
int CPMCodeDecode::GetQRCodeImage (char ** szImage, UINT *uiImageSize, int *iWidth, int *iHeight, int iLayer) {

	int		iQRCodeLengthSize;
	long	lColorCode;
	int		iRet;

	// 補正画像あり
	if (m_szQRCodeImage != NULL && m_uiQRCodeImageSize != 0){
#if PM_CODE_COLOR
		if (g_ColorCodeSetting == 0) {										// 定義ファイル
			lColorCode = g_ColorCodeSettingDefinition [iLayer];	
		} else {															// デフォルト値
			if (m_iLayer == 3) {											// 層数は３
				lColorCode = g_ColorCodeSetting3 [iLayer];	
			} else if (m_iLayer >9) {										// 層数は１０〜２４
				lColorCode = g_ColorCodeSetting10to24 [iLayer];	
			} else {														// 層数は３〜９
				lColorCode = g_ColorCodeSetting4to9 [iLayer];	
			}
		}
#else
		lColorCode = g_PMCodeColor [iLayer];								// PMコード設定のカラーコード
#endif
		if (lColorCode == 0) {												// カラーコード設定がされていない場合はエラーとする
			return RESULT_ERROR_NON_SUPORT_COLOR;
		}
		iQRCodeLengthSize	= CalcPitch (BIT_COUNT_8, m_iSymbolSize);
		m_uiQRCodeImageSize	= iQRCodeLengthSize * m_iSymbolSize;
																				// ＱＲデータ用メモリの確保
		if (m_szQRCodeImage != NULL){
			free (m_szQRCodeImage);
			m_szQRCodeImage = NULL;
		}
		m_szQRCodeImage = (char *)malloc (m_uiQRCodeImageSize + 1);
		if (m_szQRCodeImage == NULL) {											// メモリ確保失敗
			return 0;
		}
		memset (m_szQRCodeImage, '\0', m_uiQRCodeImageSize + 1);
																			// ＰＭコード→ＱＲコードへの単層分解
		iRet = PMCodeToQRCode (m_szBaseImage, m_uiBaseImageSize, lColorCode
										, m_szQRCodeImage, m_uiQRCodeImageSize, m_Width);
		if(RESULT_OK != iRet){
			return iRet;
		}
		*szImage		= m_szQRCodeImage;									// ＱＲ画像のアドレスセット
		*uiImageSize	= m_uiQRCodeImageSize;								// ＱＲ画像のサイズセット
		*iWidth			= m_Width;											// ＱＲ画像の幅セット
		*iHeight		= m_Height;											// ＱＲ画像の高さセット
	} else {
		return RESULT_ERROR_DECODE_YET;										// 復号未完了
	}
	return RESULT_OK;
}


