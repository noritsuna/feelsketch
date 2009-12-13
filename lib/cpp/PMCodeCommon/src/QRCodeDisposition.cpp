#include "QRCodeDisposition.h"
#include "global.h"
#include "define.h"
//#include "CtrlBMP.h"

// ------------------------------------------------------------------------- //
// 機能概要			:コンストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CQRCodeDisposition::CQRCodeDisposition () {

}

// ------------------------------------------------------------------------- //
// 機能概要			:デストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CQRCodeDisposition::~CQRCodeDisposition () {

}

// ------------------------------------------------------------------------- //
// 機能概要			:画像データセット										 //
// 引数				:szData				:データ								 //
//					:uiDataSize			:サイズ								 //
//					:iWordSize			:コード内データサイズ				 //
//					:iSymbolSize		:シンボルサイズ						 //
//					:iRSLevel			:誤り訂正レベル						 //
// 戻り値			:RESULT_OK												 //
//					:RESULT_ERROR_SECURE_MEMORY								 //
// 備考				:画像を保持する必要が無い為、本関数にて解析を行います	 //
//					:データ取得失敗の場合、０を返します。					 //
// ------------------------------------------------------------------------- //
int CQRCodeDisposition::SetCodeWordPattern (char * szData, int iDataSize
											, int iWordSize, int iSymbolSize, int iRSLevel) {

    int iX			= iSymbolSize;											// ＱＲコードの右下から開始する
    int iY			= iSymbolSize - 1;										// ＱＲコードの右下から開始する
    int iDirectionX = 1;													// ｘ軸配置向き
    int iDirectionY = 1;													// ｙ軸配置向き

	if (iDataSize > iWordSize) {											// データサイズが格納サイズ以上の場合はエラーとする
		return 0;
	}

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
			g_byModuleData[iX][iY] = (szData[i] & (1 << (7 - j))) ? '\x02' : '\x00';
        }
    }

	return RESULT_OK;
}

