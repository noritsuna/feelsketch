#ifndef     __PM_CODE_ENCODE_H__
#define     __PM_CODE_ENCODE_H__

#include "define.h"
#include "QRCodeToPMCode.h"
#include "PMCodeMakeHeader.h"
#include "ImageUtil.h"

// ------------------------------------------------------------------------- //
// PMコードリーダ機能基底クラス												 //
// ------------------------------------------------------------------------- //
class CPMCodeEncode : public CQRCodeToPMCode, CPMCodeMakeHeader
{
// コンストラクション
public:
	CPMCodeEncode ();
	~CPMCodeEncode ();

	int		SetPMCodeData (char *szData, UINT uiDataSize, int iLayer, int iVersion, int iRSLevel, BOOL bMaskAuto, int iModuleSize);
																			// 暗号化元データのセット
	int 	SetPMCodeData (char *data, UINT data_size, int layer
						  , int version, int error_correction, BOOL use_mask, int module_size
						  , char *extension, char *note);
	int		EncodePMCode ();												// 暗号化処理
																			// ＰＭコード画像の取得
	int		GetPMCodeImageData (char **szImage, UINT *uiImageSize, int *iWidth, int *iHeight);
																			// データがＰＭコードに収まるかのチェック
	BOOL	CheckDataSizeToPMCode (UINT uiDataSize, int iLayer, int iVersion, int iRSLevel);

	// ＱＲコード画像取得
	UINT	GetQRCodeSize (int iLayer);										// QRコード画像サイズの取得
	int		GetQRCodeImageData (char **szImage, UINT *uiImageSize, int *iWidth, int *iHeight, int iLayer);

private:
	char		*m_szEncodeData;											// 暗号化（元）データ
	UINT		m_uiDataSize;												// データサイズ
	char		*m_szPMCodeImage;											// ＰＭコード画像
	UINT		m_uiPMCodeImageSize;										// ＰＭコード画像サイズ
	char		*m_szQRCodeImage [MAX_LAYERSIZE];							// ＱＲコード画像
	UINT		m_uiQRCodeImageSize [MAX_LAYERSIZE];						// ＱＲコード画像サイズ
	int			m_iWidth;													// 画像横ピクセルサイズ
	int			m_iHeight;													// 画像縦ピクセルサイズ
	int			m_iLayer;													// 層数
	int			m_iVersion;													// 型番
	int			m_iSymbolSize;												// シンボルサイズ
	int			m_iRSLevel;													// 誤り訂正レベル
	BOOL		m_bMaskAuto;												// マスクの自動/手動フラグ
	int			m_iModuleSize;												// モジュールサイズ
};
#endif
