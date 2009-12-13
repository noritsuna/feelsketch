#include "define.h"
#include "PMCodeAnalysis.h"
#include "Global.h"


// ------------------------------------------------------------------------- //
// 本関数に渡されるＰＭコード画像は補正後の画像とします。
// 
// ------------------------------------------------------------------------- //

// ------------------------------------------------------------------------- //
// 機能概要			:コンストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CPMCodeAnalysis::CPMCodeAnalysis () {

	m_iLayerCount = 0;
}

// ------------------------------------------------------------------------- //
// 機能概要			:デストラクタ											 //
// 引数				:なし													 //
// 戻り値			:なし													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
CPMCodeAnalysis::~CPMCodeAnalysis () {

}

// ------------------------------------------------------------------------- //
// 機能概要			:層数の取得												 //
// 引数				:szImage		:ＰＭ画像データ							 //
//                  :iImageSize		:ＰＭ画像データサイズ					 //
// 戻り値			:層数													 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
int CPMCodeAnalysis::GetLayerCount (char *szImage, UINT iImageSize, int iSymbolSize) {

	char	szbuf	[32];
	int		iX, iY;
	int		iDataPos;
	int		iBitSize;
	int		iLineSize;
	char	cTemp [2];

// 層数の取得
	iBitSize	= BIT_COUNT_24 / 8;
	iLineSize	= (int)CalcPitch (BIT_COUNT_24, iSymbolSize);

	memset (szbuf, '\0', sizeof (szbuf));
	// データ取得
	for (int i = 0; i < LAYER_BIT; i ++) {
		iX = (LAYER_CODE_POS_X * iBitSize);
		iY = (9 + i);
		iDataPos = (iY * iLineSize) + iX;									// 該当位置座標の計算
		memcpy (&szbuf [i * PM_CODE_BIT_SIZE], &szImage [iDataPos], PM_CODE_BIT_SIZE);
	}
	m_iLayerCount = 0;

	// データ入れ替え（画像はRGBとして認識しているが、ビットマップデータはBGRの順のため入れ替える）
	// 時間短縮のために、対応表自体を入れ替えれば、処理が少し減るかな・・・
	for (int i = 0; i < LAYER_BIT; i ++) {
		cTemp [0] = szbuf [i * iBitSize];
		szbuf [i * iBitSize] = szbuf [(i * iBitSize) + 2];
		szbuf [(i * iBitSize) + 2] = cTemp [0];
	}

	// 二値化する
	for (int i = 0; i <= PM_CODE_HEADER_LAYER_CODE; i ++) {
		if (((BYTE)szbuf [i]) < (0xFF / 2)) {
			szbuf [i] = 0x00;
		} else {
			szbuf [i] = '\xFF';
		}
	}

	for (int i = 0; i <= PM_CODE_HEADER_LAYER_CODE; i ++) {
		// 取得したデータから該当する層数を検索する
		if (memcmp (szbuf, &g_szLayerSetting [i][0], LAYER_CODE_BUF_SIZE) == 0) {
			m_iLayerCount = i + MIN_LAYERSIZE;								// 最小層数は３から
			break;
		}
	}
	return m_iLayerCount;
}

// ------------------------------------------------------------------------- //
// 機能概要			:カラーコードの取得										 //
// 引数				:lColorCode		:色コード								 //
// 戻り値			:RESULT_OK												 //
// 備考				:ＰＭコード画像に設定してある色は１層〜２０層のみ		 //
//					:ＰＭコードに設定された層数のみ取得します				 //
// ------------------------------------------------------------------------- //
int	CPMCodeAnalysis::GetColorCode (long * lColorCode, char *szImage, UINT iImageSize, int iSymbolSize, int iVersion) {
	
	char	szbuf	[32];
	char	szTemp	[32];
	int		iX, iY;
	int		iDataPos;
	int		iBitSize;
	int		iLineSize;
	char	cTemp [2];
	long	lWorkColor [COLORTABLE_SIZE];

	// 色コードの取得（層数分だけ取得を行う）
	iBitSize	= BIT_COUNT_24 / 8;
	iLineSize	= (int)CalcPitch (BIT_COUNT_24, iSymbolSize);
	// 傾き補正→色コード→色補正→層数取得の順で行う必要があるため
	// 層数は使用できない。テーブルの最大数としておく
	if (iVersion < 3) {
		g_ColorCodeTableSize = PM_CODE_HEADER_COLOR_CODE;						// １or２型：色識別コードサイズは既存
	} else {
		g_ColorCodeTableSize = COLORTABLE_SIZE;									// ３型以降：色識別コードサイズは拡張版
	}

	for (int i = 0; i < g_ColorCodeTableSize; i ++) {
		if (i < PM_CODE_HEADER_COLOR_CODE) {
			iX = (COLOR_CODE_POS_X - (i / 4)) * iBitSize;
			iY = (9 + (i % 4));
		} else if (i >= PM_CODE_HEADER_COLOR_CODE && i < PM_CODE_HEADER_COLOR_CODE2){
			iX = (LAYER_CODE_POS_X - ((i - PM_CODE_HEADER_COLOR_CODE) / 4)) * iBitSize;
			iY = (13 + ((i - PM_CODE_HEADER_COLOR_CODE) % 4));
		} else {
			iX = (LAYER_CODE_POS_X - ((i - PM_CODE_HEADER_COLOR_CODE2) / 4)) * iBitSize;
			iY = (17 + ((i - PM_CODE_HEADER_COLOR_CODE2) % 4));
		}
		iDataPos = (iY * iLineSize) + iX;									// 該当位置座標の計算
		memcpy (szbuf, &szImage [iDataPos], PM_CODE_BIT_SIZE);
		memset (szTemp, '\0', sizeof (szTemp));
		cTemp [0]= szbuf [0];
		szbuf [0] = szbuf [2];
		szbuf [2] = cTemp [0];

		for (int j = 0 ; j < PM_CODE_BIT_SIZE; j ++) {
			b1th2 (szbuf [j], &szTemp [j * 2]);
		}
		lWorkColor [i] = AscHexToLong (szTemp, PM_CODE_BIT_SIZE * 2);
	}

	for (int i = 0; i < COLORTABLE_SIZE; i ++){
//	for (int i = 0; i < m_iLayerCount; i ++){
		lColorCode [i] = lWorkColor [i];
	}
	return RESULT_OK;
}
