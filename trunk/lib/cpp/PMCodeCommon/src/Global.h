#ifndef GLOBAL_H
#define	GLOBAL_H

#include "define.h"
#include <string.h>
#include <stdlib.h>

// ------------------------------------------------------------------------- //
// 機能概要			:１行のビット長計算										 //
// 引数				:BitCount			:ビットカウント(8,16,24,32のみ)		 //
//					:Width				:幅									 //
// 戻り値			:１行のビット長											 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
long CalcPitch (short BitCount,long Width);


// グローバル関数定義
extern LONG			AscHexToLong(PSTR pszData, int ilength);						// １６進文字列→longへの変換
extern void			b1th2( unsigned char dat, char *p );
extern unsigned char h2tb1( char *p );

extern BOOL			MakeCheckFile(char * szFile, char * szImage, int iWidth, int iHeight, BOOL iStartTop);
extern void			MakeColorTable (int iColor, int iLayer);

// グローバル変数定義
extern char			g_CurrentPath [MAX_PATH];
extern int			g_MaskSetting [MAX_LAYERSIZE];									// マスク設定
extern long			g_ColorCodeSettingDefinition [MAX_LAYERSIZE];					// 定義ファイル用色コード設定
extern long			g_ColorCodeSetting3 [3];										// ３層デフォルト色コード設定
extern long			g_ColorCodeSetting4to9 [9];										// ４〜９層デフォルト色コード設定
extern long			g_ColorCodeSetting10to24 [MAX_LAYERSIZE];						// １０〜２４層デフォルト色コード設定
extern long			g_ColorCodeTable [COLORTABLE_SIZE + 1];							// カラーテーブル
extern int			g_ColorCodeTableSize;											// カラーテーブルの使用サイズ
extern char			g_IniFilePath [MAX_PATH];										// ＩＮＩファイルパス
extern int			g_SelectMenuCode;												// 現在表示選択中のコード
extern int			g_ColorCodeSetting;												// 選択されているカラーコード設定
extern int			g_ThreeValueThreshold;											// ３値化閾値
extern int			g_ColorCodeThreshold;											// カラーコード閾値
extern int			g_CorrectSlopeThreshold;										// 傾き補正閾値
extern int			g_CorrectColorThreshold;										// 色補正閾値
extern BYTE			g_byModuleData [MAX_MODULESIZE][MAX_MODULESIZE];
extern long			g_PMCodeColor [COLORTABLE_SIZE + 1];
//extern long			g_PMCodeColor [PM_CODE_HEADER_COLOR_CODE];
extern int			g_iLevelingRate;												// 平均化率
extern int			g_iCheckFileOutput;												// 検証用ファイルの出力
// ------------------------------------------------------------------------- //
// 構造体																	 //
// ------------------------------------------------------------------------- //
typedef struct tagRS_BLOCKINFO														// 誤り訂正
{
	int ncRSBlock;																	// ＲＳブロック数
		int ncAllCodeWord;															// ブロック内コードワード数
		int ncDataCodeWord;															// データコードワード数(コードワード数 - ＲＳコードワード数)

} RS_BLOCKINFO, *LPRS_BLOCKINFO;

typedef struct tagQR_VERSIONINFO													// QRコードバージョン(型番)関連情報
{
	int nVersionNo;																	// バージョン(型番)番号(1〜40)
	int ncAllCodeWord;																// 総コードワード数

	// 以下配列添字は誤り訂正率(0 = L, 1 = M, 2 = Q, 3 = H)
	int ncDataCodeWord [4];															// データコードワード数(総コードワード数 - ＲＳコードワード数)

	int ncAlignPoint;																// アライメントパターン座標数
	int nAlignPoint [6];															// アライメントパターン中心座標

	RS_BLOCKINFO RS_BlockInfo1 [4];													// ＲＳブロック情報(1)
	RS_BLOCKINFO RS_BlockInfo2 [4];													// ＲＳブロック情報(2)

} QR_VERSIONINFO, *LPQR_VERSIONINFO;

typedef struct tagPOINT {
   LONG x;
   LONG y;
} POINT;

extern QR_VERSIONINFO QR_VersonInfo [41];

extern char g_szLayerSetting [22][12];

#endif
