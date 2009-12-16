#ifndef     __QR_CODE_RESTRATION_H__
#define     __QR_CODE_RESTRATION_H__

#include "define.h"


// ------------------------------------------------------------------------- //
// QRコードデータ復元クラス													 //
// ------------------------------------------------------------------------- //
class CQRCodeRestoration
{
// コンストラクション
public:
	CQRCodeRestoration ();
	~CQRCodeRestoration ();

	int SetCodeWordPattern (char * szImage, UINT uiImageSize
					, int iRSLevel, int iWordSize, int iSymbolSize);				// 画像データセット	
	int GetCodeData (char * szData, int iDataSize);									// データ取得

private:
	char	*m_szQRData;															// 取得データ
	int		m_iDataSize;															// データサイズ
};
#endif
