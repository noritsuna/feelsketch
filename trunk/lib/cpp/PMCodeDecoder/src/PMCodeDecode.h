#ifndef     __PM_CODE_DECODE_H__
#define     __PM_CODE_DECODE_H__

#include "PMCodeAnalysis.h"
#include "PMCodeToQRCodes.h"

// 作業Ｎｏ A-011

// ------------------------------------------------------------------------- //
// ＰＭコード複合クラス														 //
// 本クラスにて元画像、補正後画像、復号データを保持します					 //
// ＱＲコードの画像は１層分のみ保持し、層の変更時は毎回取り直します			 //
// ------------------------------------------------------------------------- //
class CPMCodeDecode : public CPMCodeAnalysis, CPMCodeToQRCodes
{
// コンストラクション
public:
	CPMCodeDecode ();
	~CPMCodeDecode ();

	// 基準色の設定
	void	SetBaseColor (int *baseColors, int layer);
	// ＰＭコード入力，復号処理
	int		SetBaseImage (char *szImage, UINT uiImageSize, int iWidth, int iHeight);
																			// 元画像のセット
	int		DecodePMCode ();												// 復号処理
	int		GetPMCodeDecodeData (char *szData, UINT uiBufferSize);			// 復号したデータの取得

	// ＱＲコード画像取得
	UINT	GetQRCodeSize (int iLayer);										// QRコード画像サイズの取得
	int		GetQRCodeImage (char **szImage, UINT *uiImageSize, int *iWidth, int *iHeight, int iLayer);
																			// QRコード画像の取得

private:
	char		*m_szBaseImage;												// 元画像
	UINT		m_uiBaseImageSize;											// 元画像サイズ
//	char		*m_szCorrectImage;											// 補正画像
//	UINT		m_uiCorrectImageSize;										// 補正画像サイズ
	char		*m_szQRCodeImage;											// ＱＲコード画像
	UINT		m_uiQRCodeImageSize;										// ＱＲコード画像サイズ
	char		*m_szDecodeData;											// 復号データ
	UINT		m_uiDecodeDataSize;											// 復号データサイズ
	int			m_Width;													// 画像横ピクセルサイズ
	int			m_Height;													// 画像縦ピクセルサイズ
//	int			m_CorrectWidth;												// 補正画像横ピクセルサイズ
//	int			m_CorrectHeight;											// 補正画像縦ピクセルサイズ
	int			m_iLayer;													// 層数
	int			m_iVersion;													// 型番
	int			m_iSymbolSize;												// シンボルサイズ
	int			m_iRSLevel;													// 誤り訂正レベル
	int			m_iMaskPattern [MAX_LAYERSIZE];								// マスクパターン
};


#endif
