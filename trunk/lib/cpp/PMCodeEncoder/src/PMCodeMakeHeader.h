#ifndef     __PM_CODE_MAKE_HEADER_H__
#define     __PM_CODE_MAKE_HEADER_H__

#define LAYER_CODE_BUF_SIZE				12

#define LAYER_BIT						4
#define LAYER_CODE_POS_X				5
#define COLOR_CODE_POS_X				4

#include "define.h"

// ------------------------------------------------------------------------- //
// ＰＭコード解析クラス														 //
// ------------------------------------------------------------------------- //
class CPMCodeMakeHeader
{
// コンストラクション
public:
	CPMCodeMakeHeader ();
	~CPMCodeMakeHeader ();
																				// ＰＭコードヘッダのセット
	int SetPMCodeHeader (char *szImage, UINT iImageSize, int iSymbolSize, int iLayer);			

private:
	int			m_iLayer;													// 層数
	int			m_iVersion;													// 型番
	int			m_iSymbolSize;												// シンボルサイズ
	int			m_iRSLevel;													// 誤り訂正レベル

};
#endif