#ifndef     __PM_CODE_ANALYSIS_H__
#define     __PM_CODE_ANALYSIS_H__

#define LAYER_CODE_BUF_SIZE				12

#define LAYER_BIT						4
#define LAYER_CODE_POS_X				5
#define COLOR_CODE_POS_X				4

#include "define.h"

// 作業Ｎｏ A-009

// ------------------------------------------------------------------------- //
// ＰＭコード解析クラス														 //
// ------------------------------------------------------------------------- //
class CPMCodeAnalysis
{
// コンストラクション
public:
	CPMCodeAnalysis ();
	~CPMCodeAnalysis ();

	int GetLayerCount (char *szImage, UINT iImageSize, int iSymbolSize);
																// 層数の取得
	int	GetColorCode (long * lColorCode, char *szImage, UINT iImageSize, int iSymbolSize, int iVersion);
																// 色コードの取得
private:
	int	m_iLayerCount;
};
#endif
