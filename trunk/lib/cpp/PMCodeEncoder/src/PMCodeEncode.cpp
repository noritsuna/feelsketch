#include "define.h"
#include "Global.h"
#include "QRCodeEncode.h"
#include "PMCodeEncode.h"
//#include "CtrlBMP.h"
#include "ReedSolomon.h"

#include <stdio.h>

#define MARGIN	4


// ------------------------------------------------------------------------- //
// 機能概要			:コンストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:初期化処理												 //
// ------------------------------------------------------------------------- //
CPMCodeEncode::CPMCodeEncode () {

	m_szEncodeData		= NULL;
	m_uiDataSize		= 0;
	m_szPMCodeImage		= NULL;
	m_uiPMCodeImageSize	= 0;
	for (int i =0; i < MAX_LAYERSIZE; i ++){
		m_szQRCodeImage	[i]	= NULL;
		m_uiQRCodeImageSize	[i] = 0;
	}
}

// ------------------------------------------------------------------------- //
// 機能概要			:デストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:確保したままにならないようメモリの開放を行う			 //
// ------------------------------------------------------------------------- //
CPMCodeEncode::~CPMCodeEncode () {

	if (m_szEncodeData != NULL) {
		free (m_szEncodeData);
		m_szEncodeData	= NULL;
		m_uiDataSize	= 0;
	}

	if (m_szPMCodeImage != NULL) {
		free (m_szPMCodeImage);
		m_szPMCodeImage	= NULL;
		m_uiDataSize	= 0;
	}

	for (int i = 0; i < MAX_LAYERSIZE; i ++){
		if (m_szQRCodeImage != NULL) {
			free (m_szQRCodeImage [i]);
			m_szQRCodeImage	[i]		= NULL;
			m_uiQRCodeImageSize	[i] = 0;
		}
	}
}

// ------------------------------------------------------------------------- //
// 機能概要			:暗号化元データのセット									 //
// 引数				:szData				:入力データ（暗号化元）				 //
//                  :uiDataSize			:入力データサイズ					 //
//                  :iLayer				:層数								 //
//                  :iVersion			:型番								 //
//                  :iRSLevel			:誤り訂正レベル						 //
//					:iModuleSize		:モジュールサイズ					 //
// 戻り値			:RESULT_OK												 //
//                  :RESULT_ERROR_SECURE_MEMORY								 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
int CPMCodeEncode::SetPMCodeData (char *szData, UINT uiDataSize, int iLayer
								  , int iVersion, int iRSLevel, BOOL bMaskAuto, int iModuleSize) {

	// メモリが確保されている場合は、一度開放する
	if (m_szEncodeData != NULL) {
		free (m_szEncodeData);
		m_szEncodeData	= NULL;
		m_uiDataSize	= 0;
	}
// 2006/12/27 誤り訂正に対応する為に、テーブル数を変更
	m_szEncodeData = (char *)realloc (m_szEncodeData
		, QR_VersonInfo[iVersion].ncDataCodeWord [iRSLevel] * iLayer);		// データ用メモリの確保
//	m_szEncodeData = (char *)realloc (m_szEncodeData, uiDataSize);			// データ用メモリの確保
	if (m_szEncodeData == NULL) {											// メモリ確保失敗
		return RESULT_ERROR_SECURE_MEMORY;
	} 
	memset (m_szEncodeData, '\0'
		, QR_VersonInfo[iVersion].ncDataCodeWord [iRSLevel] * iLayer);		//メモリの初期化
//	memset (m_szEncodeData, '\0', uiDataSize);								//メモリの初期化
	memcpy (m_szEncodeData, szData, uiDataSize);							// データのセット
	m_uiDataSize = uiDataSize;												// データサイズのセット

	m_iLayer		= iLayer;												// 層数
	m_iVersion		= iVersion;												// 型番
	m_iRSLevel		= iRSLevel;												// 誤り訂正レベル
	m_bMaskAuto		= bMaskAuto;											// マスク使用状態
	m_iWidth		= VER2SYM (iVersion);									// 幅（シンボルサイズと同一）
	m_iHeight		= VER2SYM (iVersion);									// 高さ（シンボルサイズと同一）
	m_iModuleSize	= iModuleSize;											// モジュールサイズ
	return RESULT_OK;

}

int CPMCodeEncode::SetPMCodeData (char *data, UINT data_size, int layer
								  , int version, int error_correction, BOOL use_mask, int module_size
								  , char *extension, char *note) {
	char 	*szData;
	char	szHeader [32];
	memset (szHeader, '\0', sizeof (szHeader));
	char	szSize [16];
	memset (szSize, '\0', sizeof (szSize));
	char	szHexSize [4];

	// ヘッダの作成　ここから
	// 拡張子のセット（４バイト）
	if (extension != NULL) {
		if (strlen (extension) < EXT_SIZE) {
			memcpy (szHeader, extension, strlen (extension));
		} else {
			memcpy (szHeader, extension, EXT_SIZE);
		}
	}
	// サイズのセット（４バイト）
	sprintf (szSize, "%0.8X", data_size);
	for (int i = 0; i < 4; i ++) {
		szHexSize [i] = h2tb1 (&szSize [i * 2]);
	}
	memcpy (&szHeader[EXT_SIZE], szHexSize, SIZE_SIZE);
	// 備考のセット（８バイト）
	if (note != NULL) {
		if (strlen (note) < RESERVE_SIZE) {
			memcpy (&szHeader[EXT_SIZE + SIZE_SIZE], note, strlen (note));
		} else {
			memcpy (&szHeader[EXT_SIZE + SIZE_SIZE], note, RESERVE_SIZE);
		}
	}
	// ヘッダの作成　ここまで

	// データの作成
	szData = (char *)malloc (data_size + EXT_SIZE + SIZE_SIZE + RESERVE_SIZE + 1);
	memset (szData, '\0', data_size + 1);
	// ヘッダー情報をデータにセット
	memcpy (szData, szHeader, EXT_SIZE + SIZE_SIZE + RESERVE_SIZE);
	// データをセット
	memcpy (&szData[EXT_SIZE + SIZE_SIZE + RESERVE_SIZE], data, data_size + 1);

	return SetPMCodeData(szData,
						data_size + EXT_SIZE + SIZE_SIZE + RESERVE_SIZE,
						layer,
						version,
						error_correction,
						use_mask,
						module_size);
}

// ------------------------------------------------------------------------- //
// 機能概要			:データがＰＭコードに収まるかのチェック					 //
// 引数				:uiDataSize			:入力データサイズ					 //
//                  :iLayer				:層数								 //
//                  :iVersion			:型番								 //
//                  :iRSLevel			:誤り訂正レベル						 //
// 戻り値			:TRUE:収まる FALSE:収まらない							 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
BOOL CPMCodeEncode::CheckDataSizeToPMCode (UINT uiDataSize, int iLayer, int iVersion, int iRSLevel) {

	UINT	uiLayerDataSize;

	// 現状誤り訂正レベルなしで行う	

// 2006/12/13 色識別コード変更
// 2006/12/27 誤り訂正に対応する為に、テーブル数を変更
	uiLayerDataSize = QR_VersonInfo[iVersion].ncDataCodeWord [iRSLevel] * iLayer;
/*
	if (iVersion < 3) {
//		uiLayerDataSize = QR_VersonInfo[iVersion].ncAllCodeWord * iLayer;
		uiLayerDataSize = QR_VersonInfo[iVersion].ncDataCodeWord [iRSLevel] * iLayer;
	} else {
//		uiLayerDataSize = (QR_VersonInfo[iVersion].ncAllCodeWord - 6) * iLayer;
		uiLayerDataSize = (QR_VersonInfo[iVersion].ncDataCodeWord [iRSLevel] - 6) * iLayer;
	}
*/
	if (uiLayerDataSize >= uiDataSize){
		return TRUE;
	} else {
		return FALSE;
	}
}

// ------------------------------------------------------------------------- //
// 機能概要			:暗号化処理												 //
// 引数				:なし													 //
// 戻り値			:RESULT_OK												 //
//                  :RESULT_ERROR_SECURE_MEMORY								 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
int CPMCodeEncode::EncodePMCode () {

	CQRCodeEncode	cQRCodeEncode;
	CReedSolomon	cReedSolomon;
	BOOL	bRet;
	int		iRet;
	int		iWordSize;
	char	*szOneLayerData;												// １層分データバッファ
	char	*szTemp;
	int		iOneLayerDataMaxSize;
	int		iOneLayerDataSize;
	int		iQRCodeSize;
	long	lColorCode;
	int		iLength;
	int		iMaskPattern;
	int		iPos;
	int		iDataSize;
	int		iBlockSize;
	int		iCorrectPos = 0;
	int		iDataPos = 0;
	int		iCorrectDataSize;
	int		iRSBlock1;
	int		iRSBlock2;
	char	szWhite [4] = {'\xFF', '\xFF', '\xFF', '\x00'};
	char	szBlack [4] = {'\x00', '\x00', '\x00', '\x00'};
	BYTE	szCorrectBuf [256];
	BYTE	szWork [256];
	

	// データが収まるかのチェック（上位にて行っているが、念のため）
	bRet = CheckDataSizeToPMCode (m_uiDataSize, m_iLayer, m_iVersion, m_iRSLevel);
	if (bRet != TRUE) {
		return RESULT_ERROR_SIZE_TO_SETTING;
	}
	m_iSymbolSize  = VER2SYM(m_iVersion);

	// ＱＲコードバッファが確保されていれば、開放しておく
	for (int i = 0; i < MAX_LAYERSIZE; i ++){
		if (m_szQRCodeImage != NULL) {
			free (m_szQRCodeImage [i]);
			m_szQRCodeImage	[i]	= NULL;
			m_uiQRCodeImageSize	[i] = 0;
		}
	}

	// １層データ用バッファの確保
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
	szOneLayerData = (char *)malloc(iWordSize + 1);
//	szOneLayerData = (char *)malloc(QR_VersonInfo[m_iVersion].ncAllCodeWord + 1);
	if (szOneLayerData == NULL){
		return RESULT_ERROR_SECURE_MEMORY;
	}

	// 現状誤り訂正レベルなしで行う
	// 2006/11/27 色識別コード拡張
//	iOneLayerDataMaxSize = iWordSize;
// 2006/12/27 誤り訂正に対応する為に、テーブル数を変更
	iOneLayerDataMaxSize = QR_VersonInfo[m_iVersion].ncDataCodeWord [m_iRSLevel];
/*
	if (m_iVersion < 3) {
		iOneLayerDataMaxSize = QR_VersonInfo[m_iVersion].ncDataCodeWord [m_iRSLevel];
	} else {
		iOneLayerDataMaxSize = QR_VersonInfo[m_iVersion].ncDataCodeWord [m_iRSLevel] - 6;
	}
*/
	for (int i = 0; i < m_iLayer; i ++) {
		// 2006/11/27 色識別コード拡張
		memset (szOneLayerData, '\0', iWordSize + 1);
//		memset (szOneLayerData, '\0', QR_VersonInfo[m_iVersion].ncAllCodeWord + 1);

// 2006/12/26 誤り訂正に対応 -----------------------------------------------------------------------------------
		// データの分解処理
		if ((iOneLayerDataMaxSize * (i + 1)) < (int)m_uiDataSize) {
			iOneLayerDataSize = iOneLayerDataMaxSize;
		} else {
			iOneLayerDataSize = m_uiDataSize - (iOneLayerDataMaxSize * i);
		}
		if (iOneLayerDataSize < 0) {
			iOneLayerDataSize = 0;
		}
		iDataPos = 0;
		iDataSize = iOneLayerDataSize;										// １層分のデータサイズをセット
		iRSBlock1 = QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncRSBlock;
		iRSBlock2 = QR_VersonInfo[m_iVersion].RS_BlockInfo2 [m_iRSLevel].ncRSBlock;

		for (int j = 0; j < (iRSBlock1 + iRSBlock2); j ++) {
//		for (int j = 0; j < iDataSize; j ++) {
			// jをブロック位置としてＲＳブロック情報１or２のどちらを使用するか判断する
			if (iRSBlock1 > j){
				iBlockSize = QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncDataCodeWord;
				cReedSolomon.SetCorrectCodeSize (QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncAllCodeWord	- iBlockSize);
			} else {
				iBlockSize = QR_VersonInfo[m_iVersion].RS_BlockInfo2 [m_iRSLevel].ncDataCodeWord;
				cReedSolomon.SetCorrectCodeSize (QR_VersonInfo[m_iVersion].RS_BlockInfo2 [m_iRSLevel].ncAllCodeWord - iBlockSize);
			}
			memset (szWork, NULL, sizeof (szWork));
			memset (szCorrectBuf, NULL, sizeof (szCorrectBuf));

			// 各ブロックでのエンコード
			memcpy (szWork, &m_szEncodeData [iCorrectPos], iBlockSize);
			cReedSolomon.Encode (szWork, iBlockSize, szCorrectBuf, &iCorrectDataSize);
/*
			if (iDataSize < iBlockSize) {									// ブロックサイズより、データ数が少ないとき
				memcpy (szWork, &m_szEncodeData [iCorrectPos], iDataSize);
				cReedSolomon.Encode (szWork, iDataSize, szCorrectBuf, &iCorrectDataSize);
			} else {														// ブロックサイズより、データ数が多いとき
				memcpy (szWork, &m_szEncodeData [iCorrectPos], iBlockSize);
				cReedSolomon.Encode (szWork, iBlockSize, szCorrectBuf, &iCorrectDataSize);
			}
*/
			// エンコードしたデータを並べる
			memcpy (&szOneLayerData [iDataPos], szCorrectBuf, iCorrectDataSize);
			iCorrectPos += iBlockSize;
			iDataPos += iCorrectDataSize;
			iDataSize -= iBlockSize;
		}

//		memcpy (szOneLayerData, &m_szEncodeData [iOneLayerDataMaxSize * i], iOneLayerDataSize);
// -------------------------------------------------------------------------------------------------------------

		// ＱＲコードの作成
		if (m_bMaskAuto == TRUE){
			iMaskPattern = -1;
		} else {
			iMaskPattern = g_MaskSetting [i];
		}
// 2006/10/12 tsato update ----------------------------------------------
// サイズを有効サイズにすると、最後のデータが化けるため、最大サイズとする
		iRet = cQRCodeEncode.SetQRCodeData (szOneLayerData, iDataPos
										, m_iVersion, m_iRSLevel, iMaskPattern);
//		iRet = cQRCodeEncode.SetQRCodeData (szOneLayerData, iOneLayerDataMaxSize
//										, m_iVersion, m_iRSLevel, iMaskPattern);
//		iRet = cQRCodeEncode.SetQRCodeData (szOneLayerData, iOneLayerDataSize
//										, m_iVersion, m_iRSLevel, iMaskPattern);
// ----------------------------------------------------------------------
		if (iRet != RESULT_OK) {
			free (szOneLayerData);											// １層分データバッファの開放
			return bRet;
		}
		// ＱＲコードの作成
		iQRCodeSize = cQRCodeEncode.EncodeQRCode ();
		if (iQRCodeSize == 0) {
			free (szOneLayerData);											// １層分データバッファの開放
			return RESULT_ERROR_DECODE_FAILURE;
		}
		m_uiQRCodeImageSize	[i] = iQRCodeSize;								// ＱＲコードサイズのセット
		m_szQRCodeImage [i] = (char *)malloc (m_uiQRCodeImageSize [i] + 1);
		if (m_szQRCodeImage [i] == NULL){
			free (szOneLayerData);											// １層分データバッファの開放
			return RESULT_ERROR_SECURE_MEMORY;
		}
		memset (m_szQRCodeImage	[i], '\0', m_uiQRCodeImageSize [i] + 1);

		// ＱＲコード画像の取得
		iRet = cQRCodeEncode.GetQRCodeImage (m_szQRCodeImage [i], m_uiQRCodeImageSize [i]);
		if (iRet != RESULT_OK) {
			free (szOneLayerData);											// １層分データバッファの開放
			return bRet;
		}
	}
	free (szOneLayerData);													// １層分データバッファの開放

	// ＰＭコード用のメモリ確保
	iLength = CalcPitch (BIT_COUNT_24, m_iSymbolSize);
	m_uiPMCodeImageSize = iLength * m_iSymbolSize;
	if (m_szPMCodeImage != NULL){
		free (m_szPMCodeImage);
		m_szPMCodeImage = NULL;
	}
	m_szPMCodeImage = (char *)malloc (m_uiPMCodeImageSize + 1);				// ＰＭコードバッファの確保
	if (m_szPMCodeImage == NULL) {											// メモリ確保失敗
		return RESULT_ERROR_SECURE_MEMORY;
	}
	memset (m_szPMCodeImage, '\0', m_uiPMCodeImageSize + 1);				

	// ＱＲコードの重畳
	for (int i = 0; i < m_iLayer; i ++) {
		if (g_ColorCodeSetting == 0) {										// 定義ファイル
			lColorCode = g_ColorCodeSettingDefinition [i];	
		} else {															// デフォルト値
			if (m_iLayer == 3) {											// 層数は３
				lColorCode = g_ColorCodeSetting3 [i];	
			} else if (m_iLayer >9) {										// 層数は１０～２４
				lColorCode = g_ColorCodeSetting10to24 [i];	
			} else {														// 層数は３～９
				lColorCode = g_ColorCodeSetting4to9 [i];	
			}
		}
		iRet = QRCodeToPMCode (m_szQRCodeImage [i], m_uiQRCodeImageSize	[i], lColorCode
						, m_szPMCodeImage, m_uiPMCodeImageSize, m_iSymbolSize);		// ＱＲコード→ＰＭコード変換
	}

// -----------------------------------------------------------------------------------
// ３層、９層、２４層以外、または必要な層のＯＮとなる箇所を重畳しても
// 白とならない場合の為に、ＱＲコードから機能コードを貰い、もう一度
// 色の設定を行う
// -----------------------------------------------------------------------------------
	// フォーマット情報記述位置を機能モジュールからはずす
	for (int i = 0; i < 9; i ++) {
		// 　左上横　　　　　　　	左下縦
		g_byModuleData[i][8] = g_byModuleData[8][i] = '\x00';
	}

	for (int i = 0; i < 8; i ++) {
		//		右上横　　　　　　　　　　　　　　　　　　　　　左下縦
		g_byModuleData[m_iSymbolSize - 8 + i][8] = g_byModuleData[8][m_iSymbolSize - 8 + i] = '\x00';
	}

	for (int i = 0; i < m_iSymbolSize; i ++) {
		for (int j = 0; j < m_iSymbolSize; j ++) {
			if (g_byModuleData[j][m_iSymbolSize - (i + 1)] & 0x20) {
				iPos = (i * iLength) + (j * PM_CODE_BIT_SIZE);
				// 黒以外のときは白にしてやる
				if (memcmp (&m_szPMCodeImage [iPos], szBlack, PM_CODE_BIT_SIZE) != 0) {
					memcpy (&m_szPMCodeImage [iPos], szWhite, PM_CODE_BIT_SIZE);
				}
			}
		}
	}
/*
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
	
	
*/
	// ヘッダ情報（色識別コード、層数識別コード）のセット
	iRet = SetPMCodeHeader (m_szPMCodeImage, m_uiPMCodeImageSize, m_iSymbolSize, m_iLayer);
	if (iRet != RESULT_OK) {
		return bRet;
	}

	// 周りに枠を付加する
	szTemp = (char *)malloc (m_uiPMCodeImageSize);
	if (szTemp == NULL) {
		return RESULT_ERROR_SECURE_MEMORY;
	}
	memset (szTemp, '\0', sizeof (szTemp));
	memcpy (szTemp, m_szPMCodeImage, m_uiPMCodeImageSize);
	m_iWidth	+= MARGIN;
	m_iHeight	+= MARGIN;
	m_uiPMCodeImageSize = CalcPitch (BIT_COUNT_24, m_iWidth) * m_iHeight;
	m_szPMCodeImage = (char *)realloc (m_szPMCodeImage, m_uiPMCodeImageSize);
	memset (m_szPMCodeImage, '\0', m_uiPMCodeImageSize);
	for (int i = 0; i < (m_iHeight - MARGIN); i ++) {
		iPos = (i + (MARGIN / 2)) * CalcPitch (BIT_COUNT_24, m_iWidth);
		iPos += ((MARGIN / 2) * PM_CODE_BIT_SIZE);
		memcpy (&m_szPMCodeImage [iPos], &szTemp [i * iLength], iLength);
	}
	free (szTemp);

	// モジュールコードのあわせてＰＭコードを拡大する
	ImageUtil::UpScale ((unsigned char **)&m_szPMCodeImage, (m_iWidth * m_iModuleSize), (m_iHeight * m_iModuleSize), m_iWidth, m_iHeight);
	// 上下が反転しているので、反転する。
	ImageUtil::UpDownReplace((unsigned char *)m_szPMCodeImage, (m_iWidth * m_iModuleSize), (m_iHeight * m_iModuleSize));

	m_iWidth	*= m_iModuleSize;
	m_iHeight	*= m_iModuleSize;
	iLength		= CalcPitch (BIT_COUNT_24, m_iWidth);
	m_uiPMCodeImageSize = iLength * m_iHeight;

	return RESULT_OK;
}

// ------------------------------------------------------------------------- //
// 機能概要			:ＰＭコード画像の取得									 //
// 引数				:szImage			:ＰＭコード画像						 //
//                  :uiImageSize		:ＰＭコード画像サイズ				 //
//                  :iWidth				:幅									 //
//                  :iHeight			:高さ								 //
// 戻り値			:RESULT_OK												 //
//                  :RESULT_ERROR_SECURE_MEMORY								 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
int	CPMCodeEncode::GetPMCodeImageData (char **szImage, UINT *uiImageSize, int *iWidth, int *iHeight) {

	if (m_szPMCodeImage != NULL && m_uiPMCodeImageSize != 0){
		*szImage		= m_szPMCodeImage;									// ＰＭコード画像のアドレスセット
		*uiImageSize	= m_uiPMCodeImageSize;								// ＰＭコード画像のサイズセット
		*iWidth			= m_iWidth;											// ＰＭコード画像の幅セット
		*iHeight		= m_iHeight;										// ＰＭコード画像の高さセット
	} else {
		return RESULT_ERROR_ENCODE_YET;										// 復号暗号化未完了
	}
	return RESULT_OK;
}

// ------------------------------------------------------------------------- //
// 機能概要			:QRコード画像サイズの取得								 //
// 引数				:iLayer				:層番号								 //
// 戻り値			:RESULT_OK												 //
//                  :RESULT_ERROR_SECURE_MEMORY								 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
UINT CPMCodeEncode::GetQRCodeSize (int iLayer) {
	return m_uiQRCodeImageSize [iLayer];
}

// ------------------------------------------------------------------------- //
// 機能概要			:ＱＲコード画像取得										 //
// 引数				:szImage			:ＰＭコード画像						 //
//                  :uiImageSize		:ＰＭコード画像サイズ				 //
//                  :iWidth				:幅									 //
//                  :iHeight			:高さ								 //
//                  :iLayer				:層番号（－１しておく事！！			 //
// 戻り値			:RESULT_OK												 //
//                  :RESULT_ERROR_ENCODE_YET								 //
// 備考				:幅、高さのピクセルサイズはＰＭコードの様にマージンが	 //
//					:ないので、マージン分を引いておく						 //
// ------------------------------------------------------------------------- //
int	CPMCodeEncode::GetQRCodeImageData (char **szImage, UINT *uiImageSize, int *iWidth, int *iHeight, int iLayer) {

	// 最大層数以上、または選択層番号を使用していない場合エラーとする
	if ((m_iLayer - 1 < iLayer) 
		|| (m_szQRCodeImage [iLayer] == NULL) 
		|| (m_uiQRCodeImageSize [iLayer] == 0)) { 
		return RESULT_ERROR_ENCODE_YET;										// 暗号化未完了
	}
	*szImage		= m_szQRCodeImage [iLayer];								// ＱＲ画像のアドレスセット
	*uiImageSize	= m_uiQRCodeImageSize [iLayer];							// ＱＲ画像のサイズセット
	*iWidth			= (m_iWidth / m_iModuleSize) - MARGIN;					// ＱＲ画像の幅セット
	*iHeight		= (m_iHeight / m_iModuleSize) - MARGIN;					// ＱＲ画像の高さセット
	return RESULT_OK;
}

