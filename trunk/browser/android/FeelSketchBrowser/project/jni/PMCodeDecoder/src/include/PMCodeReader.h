#ifndef     __PM_CODE_READER_H__
#define     __PM_CODE_READER_H__

#include "PMCodeDecode.h"
#include "define.h"

// ------------------------------------------------------------------------- //
// PMコードリーダ機能基底クラス												 //
// ------------------------------------------------------------------------- //
class CPMCodeReader : public CPMCodeDecode
{
// コンストラクション
public:
	CPMCodeReader ();
	~CPMCodeReader ();

	UINT	DecodePMCodeImage		();								// 復号処理
	int		GetPMCodeHeader			(char *);						// PMコードのヘッダ情報取得
	int		GetPMCodeImage			(char **, UINT *, int *, int *);// PMコード画像取得
	int		GetPMCodeCorrectImage	(char **, UINT *, int *, int *);// PMコード補正画像取得
	int		GetQRCodeData			(char **, UINT *, int *, int *, int);
																	// QRコード画像取得
	int		GetDecodeData			(char *, int);					// 復号データ取得

private:
	int		m_iInputType;											// PMコード入力形式
	char	*m_szPMCodeImage;										// PMコード画像
	UINT	m_uiPMCodeImageSize;									// PMコード画像サイズ
	int		m_iHeight;
	int		m_iWidth;
};
#endif
