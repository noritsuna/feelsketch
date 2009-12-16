#ifndef     __QR_CODE_UNMASK_H__
#define     __QR_CODE_UNMASK_H__

#include "define.h"

// ------------------------------------------------------------------------- //
// QRコードマスク解除クラス													 //
// ------------------------------------------------------------------------- //
class CQRCodeUnMask
{
// コンストラクション
public:
	CQRCodeUnMask ();
	~CQRCodeUnMask ();

	void GetUnMaskQRCode (char * szImage, UINT iImageSize, int iMaskPattern
												, int iVersion);			// マスク処理
};
#endif
