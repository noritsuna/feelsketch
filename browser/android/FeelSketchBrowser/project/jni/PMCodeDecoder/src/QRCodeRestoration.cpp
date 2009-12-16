#include "QRCodeRestoration.h"
#include "global.h"
#include "define.h"

// ------------------------------------------------------------------------- //
// 機能概要			:コンストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CQRCodeRestoration::CQRCodeRestoration () {

	m_szQRData = NULL;
}

// ------------------------------------------------------------------------- //
// 機能概要			:デストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CQRCodeRestoration::~CQRCodeRestoration () {

	if(m_szQRData != NULL) {
		free (m_szQRData);
		m_szQRData = NULL;
	}
}

// ------------------------------------------------------------------------- //
// 機能概要			:画像データセット										 //
// 引数				:szImage			:ＱＲ画像データ						 //
//					:iImageSize			:画像サイズ							 //
//					:iWordSize			:コード内データサイズ				 //
//					:iSymbolSize		:シンボルサイズ						 //
//					:iRSLevel			:誤り訂正レベル						 //
// 戻り値			:RESULT_OK												 //
//					:RESULT_ERROR_SECURE_MEMORY								 //
// 備考				:画像を保持する必要が無い為、本関数にて解析を行います	 //
//					:データ取得失敗の場合、０を返します。					 //
// ------------------------------------------------------------------------- //
int CQRCodeRestoration::SetCodeWordPattern (char * szImage, UINT uiImageSize
											, int iWordSize, int iSymbolSize, int iRSLevel) {

    int iX			= iSymbolSize;											// ＱＲコードの右下から開始する
    int iY			= iSymbolSize - 1;										// ＱＲコードの右下から開始する
    int iDirectionX = 1;													// ｘ軸配置向き
    int iDirectionY = 1;													// ｙ軸配置向き
	int	iLineLength;

	iLineLength = CalcPitch (BIT_COUNT_8, iSymbolSize);

	// メモリが既に確保されている場合は、一度開放する
	if(m_szQRData != NULL) {
		free (m_szQRData);
		m_szQRData	= NULL;
		m_iDataSize = 0;
	}
	m_iDataSize = iWordSize;
	m_szQRData = (char *)malloc (m_iDataSize + 1);							// メモリ確保
	if (m_szQRData == NULL) {
		return RESULT_ERROR_SECURE_MEMORY;
	}
    memset (m_szQRData, '\0', m_iDataSize + 1);

// データ取得処理
    for (int i = 0; i < iWordSize; i ++ ){
        for (int j = 0; j < 8; j ++) {
            do {
				iX += iDirectionX;
				iDirectionX *= -1;

				if (iDirectionX < 0) {
					iY += iDirectionY;

					if (iY < 0 || iY == iSymbolSize) {
						if (iY < 0) {
							iY = 0;
						} else {
							iY = iSymbolSize - 1;
						}
						iDirectionY *= -1;

						iX -= 2;

						if (iX == 6) // タイミングパターン
							iX -= 1;
					}
				}
			// 機能モジュールはデータから除外する
            } while (g_byModuleData[iX][iY] & 0x20);
// 2006/10/11 tsato update ----------------------------------------------
            if ((szImage[(iY * iLineLength) + iX] & 0x01) == 0x01) {					// ビットがON
//            if ((szImage[(((iSymbolSize - 1) - iY) * iLineLength) + iX] & 0x01) == 0x01) {					// ビットがON
// ----------------------------------------------------------------------
                m_szQRData [i] |= 1 << (7 - j);
            }
        }
    }

// 誤り訂正
	// 現状は誤り訂正を行わないので、そのままのデータとする


	return RESULT_OK;
}

// ------------------------------------------------------------------------- //
// 機能概要			:データ取得												 //
// 引数				:szData				:データ								 //
//					:iDataSize			:データバッファサイズ				 //
// 戻り値			:RESULT_OK												 //
//					:RESULT_ERROR_DECODE_YET								 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
int CQRCodeRestoration::GetCodeData (char * szData, int iDataSize) {
	
	// 復号したデータあり
	if (m_szQRData != NULL && m_iDataSize != 0) {
		// 復号データがバッファに収まるなら、復号データ分だけセットする
		if (iDataSize >= m_iDataSize) {
			memcpy (szData, m_szQRData, m_iDataSize);
		// データバッファサイズがデータより小さい場合は、バッファサイズ分セットする
		} else {
			memcpy (szData, m_szQRData, iDataSize);
		}
	} else {
		return RESULT_ERROR_DECODE_YET;										// 復号未完了
	}
	return RESULT_OK;														// 処理完了
}
