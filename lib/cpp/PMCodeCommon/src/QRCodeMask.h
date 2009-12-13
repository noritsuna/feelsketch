#ifndef     __QR_CODE_MASK_H__
#define     __QR_CODE_MASK_H__

#include "define.h"

// ------------------------------------------------------------------------- //
// QRコードマスククラス														 //
// ------------------------------------------------------------------------- //
class CQRCodeMask
{
// コンストラクション
public:
	CQRCodeMask ();
	~CQRCodeMask ();

	void GetMaskQRCode (char * szImage, UINT iImageSize, int iMaskPattern
												, int iVersion);			// マスク解除処理
	void SetMaskQRCode (int iMaskPattern, int iVersion);					// マスク処理
};
#endif
