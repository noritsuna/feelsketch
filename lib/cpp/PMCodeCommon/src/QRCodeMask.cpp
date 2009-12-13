#include "QRCodeMask.h"
#include "define.h"
#include "global.h"
//#include "CtrlBMP.h"

// ------------------------------------------------------------------------- //
// 機能概要			:コンストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CQRCodeMask::CQRCodeMask () {

}

// ------------------------------------------------------------------------- //
// 機能概要			:デストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CQRCodeMask::~CQRCodeMask () {

}

// ------------------------------------------------------------------------- //
// 機能概要			:マスク解除処理											 //
// 引数				:szImage			:ＱＲ画像データ						 //
//					:iImageSize			:画像サイズ							 //
//					:iMaskPattern		:マスクパターン						 //
//					:byModuleData		:機能モジュールデータ				 //
//					:iVersion			:型番								 //
// 戻り値			:なし													 //
// 備考				:画像を保持する必要が無い為、本関数にて解析を行います	 //
//					:呼び出し元で画像サイズとシンボルサイズのチェックを行う！//
// ------------------------------------------------------------------------- //
void CQRCodeMask::GetMaskQRCode (char * szImage, UINT iImageSize, int iMaskPattern, int iVersion) {
	
	int		iLineLength;
	int		iSymbolSize = VER2SYM (iVersion);

	iLineLength = CalcPitch (BIT_COUNT_8, iSymbolSize);
	
	for (int i = 0; i < iSymbolSize; i ++) {
		for (int j = 0; j < iSymbolSize; j ++) {
			// 機能モジュールを除外
			if (! (g_byModuleData[j][i] & 0x20)) {
				BOOL bMask;

				switch (iMaskPattern) {
				case 0:
					bMask = ((i + j) % 2 == 0);
					break;

				case 1:
					bMask = (i % 2 == 0);
					break;

				case 2:
					bMask = (j % 3 == 0);
					break;

				case 3:
					bMask = ((i + j) % 3 == 0);
					break;

				case 4:
					bMask = (((i / 2) + (j / 3)) % 2 == 0);
					break;

				case 5:
					bMask = (((i * j) % 2) + ((i * j) % 3) == 0);
					break;

				case 6:
					bMask = ((((i * j) % 2) + ((i * j) % 3)) % 2 == 0);
					break;

				default: // case 7:
					bMask = ((((i * j) % 3) + ((i + j) % 2)) % 2 == 0);
					break;
				}
				// マスク処理
// 2006/10/11 tsato update ----------------------------------------------
				szImage[(i * iLineLength) + j] = (char)(szImage[(i * iLineLength) + j] ^ bMask);
//				szImage[(((iSymbolSize - 1) - i) * iLineLength) + j] = (char)(szImage[(((iSymbolSize - 1) - i) * iLineLength) + j] ^ bMask);
// ----------------------------------------------------------------------
			}
		}
	}
}

// ------------------------------------------------------------------------- //
// 機能概要			:マスク処理												 //
// 引数				:szImage			:ＱＲ画像データ						 //
//					:iImageSize			:画像サイズ							 //
//					:iMaskPattern		:マスクパターン						 //
//					:byModuleData		:機能モジュールデータ				 //
//					:iVersion			:型番								 //
// 戻り値			:なし													 //
// 備考				:画像を保持する必要が無い為、本関数にて解析を行います	 //
//					:呼び出し元で画像サイズとシンボルサイズのチェックを行う！//
// ------------------------------------------------------------------------- //
void CQRCodeMask::SetMaskQRCode (int iMaskPattern, int iVersion) {
	
	int	iSymbolSize = VER2SYM (iVersion);
	
	for (int i = 0; i < iSymbolSize; i ++) {
		for (int j = 0; j < iSymbolSize; j ++) {
			// 機能モジュールを除外
			if (! (g_byModuleData[j][i] & 0x20)) {
				BOOL bMask;

				switch (iMaskPattern) {
				case 0:
					bMask = ((i + j) % 2 == 0);
					break;

				case 1:
					bMask = (i % 2 == 0);
					break;

				case 2:
					bMask = (j % 3 == 0);
					break;

				case 3:
					bMask = ((i + j) % 3 == 0);
					break;

				case 4:
					bMask = (((i / 2) + (j / 3)) % 2 == 0);
					break;

				case 5:
					bMask = (((i * j) % 2) + ((i * j) % 3) == 0);
					break;

				case 6:
					bMask = ((((i * j) % 2) + ((i * j) % 3)) % 2 == 0);
					break;

				default: // case 7:
					bMask = ((((i * j) % 3) + ((i + j) % 2)) % 2 == 0);
					break;
				}
				// マスク処理
				g_byModuleData[j][i] = (BYTE)((g_byModuleData[j][i] & 0xfe) | (((g_byModuleData[j][i] & 0x02) > 1) ^ bMask));
			}
		}
	}
}
