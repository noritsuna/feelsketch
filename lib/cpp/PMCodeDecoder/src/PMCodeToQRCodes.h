#ifndef     __PM_CODE_TO_QR_CODES_H__
#define     __PM_CODE_TO_QR_CODES_H__


#include "define.h"

// 作業Ｎｏ A-010

// ------------------------------------------------------------------------- //
// ＰＭコード→QRコード変換クラス											 //
// ------------------------------------------------------------------------- //
class CPMCodeToQRCodes
{
// コンストラクション
public:
	CPMCodeToQRCodes ();
	~CPMCodeToQRCodes ();

	int PMCodeToQRCode (char *szPMCodeImage, UINT uiPMCodeImageSize, long lColorCode
					, char *szQRCodeImage, UINT uiQRCodeImageSize, int uiSymbolSize);		// ＰＭコード→ＱＲコード変換
};
#endif
