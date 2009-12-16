#ifndef     __QR_CODE_SYMBOL_H__
#define     __QR_CODE_SYMBOL_H__

#include "define.h"

// ------------------------------------------------------------------------- //
// QRコードシンボル読込クラス												 //
// 型式の読込（型番は既に分かっているので処理しない）						 //
// ------------------------------------------------------------------------- //
class CQRCodeSymbol
{
// コンストラクション
public:
	CQRCodeSymbol ();
	~CQRCodeSymbol ();

	int GetQRCodeSymbol (char * szImage, UINT iImageSize, int iSymbolSize
						  , int *iRSLevel, int *iMaskPattern);				// シンボル（型式）の取得

	int SetQRCodeSymbol (int iSymbolSize, int iRSLevel, int iMaskPattern);	// シンボル（型式）の設定

private:
	int	m_iRSLevel;												// 誤り訂正レベル
	int	m_iMaskPattern;											// マスクパターン

};
#endif
