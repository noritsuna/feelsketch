#ifndef     __QR_CODE_DECODE_H__
#define     __QR_CODE_DECODE_H__

#include "define.h"

// ------------------------------------------------------------------------- //
// QRコード複合クラス														 //
// ------------------------------------------------------------------------- //
class CQRCodeDecode
{
// コンストラクション
public:
	CQRCodeDecode ();
	~CQRCodeDecode ();

	int		SetQRCodeImage (char * szImage, UINT iImageSize, int iVersion);	// QRコードのセット
	UINT	DecodeQRCodeImage ();											// QRコードのデコード
	int		GetDecodeData (char *szData, UINT iBufferSize);					// 復号データの取得
	int		GetQRCodeStatus (int * iRSLevel);
	int		GetMaskNumber ();

private:
	void	SetFunctionModule ();											// 機能モジュールの設定
	void	SetFinderPattern(int x, int y);									// 位置検出パターン
	void	SetVersionPattern();											// 型番情報の設定
	void	SetAlignmentPattern(int x, int y);								// 位置合わせパターン配置

private:
	char	*m_QRCodeImage;													// ＱＲコード画像
	UINT	m_uiQRCodeImageSize;											// ＱＲコード画像サイズ
	char	*m_szDecodeData;												// 復号データ
	UINT	m_uiDecodeDataSize;												// 復号データサイズ
	int		m_iVersion;														// 型番
	int		m_iSymbolSize;													// シンボルサイズ
	int		m_iRSLevel;														// 誤り訂正レベル
	int		m_iMaskPattern;													// マスクパターン

};
#endif
