#ifndef     __QR_CODE_TO_PM_CODE_H__
#define     __QR_CODE_TO_PM_CODE_H__

#include "define.h"

// 作業Ｎｏ B-007

// ------------------------------------------------------------------------- //
// QRコード→ＰＭコード変換クラス											 //
// ------------------------------------------------------------------------- //
class CQRCodeToPMCode
{
// コンストラクション
public:
	CQRCodeToPMCode ();
	~CQRCodeToPMCode ();

	int QRCodeToPMCode (char *szQRCodeImage, UINT uiQRCodeImageSize, long lColorCode
					, char *szPMCodeImage, UINT uiPMCodeImageSize, int uiSymbolSize);		// ＱＲコード→ＰＭコード変換
};
#endif
