#include "define.h"
#include "Global.h"
#include "PMCodeReader.h"

// ------------------------------------------------------------------------- //
// 機能概要			:コンストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CPMCodeReader::CPMCodeReader () {

	m_szPMCodeImage = NULL;
}

// ------------------------------------------------------------------------- //
// 機能概要			:デストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CPMCodeReader::~CPMCodeReader () {

}


// ------------------------------------------------------------------------- //
// 機能概要			:復号処理												 //
// 引数				:なし													 //
// 戻り値			:復号データサイズ										 //
// 備考				:復号できなかった場合は、結果が０以下となります			 //
// ------------------------------------------------------------------------- //
UINT CPMCodeReader::DecodePMCodeImage () {

	int		DataSize;

	DataSize = DecodePMCode ();
	return DataSize;														// 復号データサイズを返す
}

// ------------------------------------------------------------------------- //
// 機能概要			:QRコード画像取得										 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
int CPMCodeReader::GetQRCodeData (char ** szData, UINT *uiImageSize, int *iWidth, int *iHeight, int iLayer) {

	int		iRet;

	iRet = GetQRCodeImage (szData, uiImageSize, iWidth, iHeight, iLayer);
	if(RESULT_OK != iRet){
		return iRet;
	}
	return 0;
}

// ------------------------------------------------------------------------- //
// 機能概要			:復号結果の取得											 //
// 引数				:szData				:結果格納用バッファ					 //
//                  :szDataSize			:結果格納用バッファサイズ			 //
// 戻り値			:RESULT_OK												 //
//					:RESULT_ERROR_DECODE_YET								 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
int	CPMCodeReader::GetDecodeData (char *szData, int szDataSize) {

	int iRet;
	iRet = GetPMCodeDecodeData (szData, szDataSize);
	return iRet;
}

