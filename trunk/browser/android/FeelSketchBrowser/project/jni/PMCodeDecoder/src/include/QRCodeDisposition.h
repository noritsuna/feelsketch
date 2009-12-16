#ifndef     __QR_CODE_DISPOSITION_H__
#define     __QR_CODE_DISPOSITION_H__

// ------------------------------------------------------------------------- //
// QRコードデータ暗号クラス													 //
// ------------------------------------------------------------------------- //
class CQRCodeDisposition
{
// コンストラクション
public:
	CQRCodeDisposition ();
	~CQRCodeDisposition ();

	int SetCodeWordPattern (char * szData, int iDataSize,int iRSLevel
						, int iWordSize, int iSymbolSize);						// 画像データセット	
private:
};
#endif