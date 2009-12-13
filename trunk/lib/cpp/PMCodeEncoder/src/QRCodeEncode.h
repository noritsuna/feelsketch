#ifndef     __QR_CODE_ENCODE_H__
#define     __QR_CODE_ENCODE_H__

#include "define.h"

// ------------------------------------------------------------------------- //
// QRコード複合クラス														 //
// ------------------------------------------------------------------------- //
class CQRCodeEncode
{
// コンストラクション
public:
	CQRCodeEncode ();
	~CQRCodeEncode ();

	int		SetQRCodeData (char * szData, UINT iDataSize, int iVersion
						, int iRSLevel, int iMaskPattern);					// QRコード情報のセット
	UINT	EncodeQRCode ();												// QRコードのエンコード
	int		GetQRCodeImage (char *szImage, UINT iBufferSize);				// QRコード画像の取得

private:
	void	SetFunctionModule ();											// 機能モジュールの設定
	void	SetFinderPattern(int x, int y);									// 位置検出パターン
	void	SetVersionPattern();											// 型番情報の設定
	void	SetAlignmentPattern(int x, int y);								// 位置合わせパターン配置
	int		CountPenalty();													// 失点数の取得

private:
	char	*m_QRCodeImage;													// ＱＲコード画像
	UINT	m_uiQRCodeImageSize;											// ＱＲコード画像サイズ
	char	*m_szEncodeData;												// エンコードデータ
	UINT	m_uiEncodeDataSize;												// エンコードデータサイズ
	int		m_iVersion;														// 型番
	int		m_iRSLevel;														// 誤り訂正レベル
	int		m_iMaskPattern;													// マスクパターン
	int		m_iSymbolSize;													// シンボルサイズ
};
#endif
