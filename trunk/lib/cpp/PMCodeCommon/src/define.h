#ifndef     __DEFINE_H__
#define     __DEFINE_H__


#define BOOL 								int
#define CString								string
#define	LONG								long
#define PSTR								char *
#define BYTE								unsigned char
#define UINT								unsigned int
#define LPBYTE								unsigned char *

#define TRUE								0
#define FALSE								1

#define MAX_PATH							128
#define INT32_MAX 							0x7fffffffL


// ------------------------------------------------------------------------------------------------- //
//											定数宣言												 //
// ------------------------------------------------------------------------------------------------- //
// ビットサイズ用
#define BIT_COUNT_8						8												// ２５６色
#define BIT_COUNT_16					16												// ６５５３５色
#define BIT_COUNT_24					24												// １６７７７２１６色
#define BIT_COUNT_32					32												// 上記と同じ・・１バイトの使用用途は不明

// 平均化タイプ
#define	LEVELING_TYPE_CENTER			0												// 中心のみを使用して平均化
#define LEVELING_TYPE_CENTER_VICINITY	1												// 中央付近
#define	LEVELING_TYPE_ALL				2												// 全ビットを平均化




#define PM_CODE_COLOR						1		// 0:層色テーブルを使用
													// 1:画面の色設定を使用
#define	INI_FILE							"PMCode.INI"

// 共通エラーコード
#define RESULT_OK							0		// 処理成功
#define RESULT_ERROR_SECURE_MEMORY			-1		// メモリ確保失敗
#define RESULT_ERROR_DECODE_YET				-2		// 未復号処理
#define RESULT_ERROR_IMAGEFILE_READ			-3		// 画像読込失敗
#define RESULT_ERROR_NON_SUPORT_FILE		-4		// 未対応ファイル
#define RESULT_ERROR_NON_SUPORT_COLOR		-5		// 画像未対応
#define RESULT_ERROR_IMAGEFILE_READ_YET		-6		// 画像未読込
#define RESULT_ERROR_MARK_UNDETECTED		-7		// 位置補正マーク未検出
#define RESULT_ERROR_QR_CODE_HEADER			-8		// ＱＲコードのヘッダーエラー
#define RESULT_ERROR_COLLECT_FAILTURE		-9		// 補正失敗
#define RESLUT_ERROR_LAYER_RESOLUTION		-10		// 単層分解失敗
#define RESULT_ERROR_ANALYSIS_FAILURE		-11		// 解析失敗
#define RESULT_ERROR_ANALYSIS_LAYER			-12		// 層解析失敗
#define RESULT_ERROR_ANALYSIS_QR_IMAGE		-13		// QRImage解析失敗
#define RESULT_ERROR_ANALYSIS_QR_STATUS		-14		// QRStatus解析失敗
#define RESULT_ERROR_ANALYSIS_QR_DECODE		-15		// QRDecode解析失敗
#define RESULT_ERROR_ANALYSIS_QR_BIG_BLOCK_DECODE		-16		// QRブロック(でかいほう)解析失敗
#define RESULT_ERROR_ANALYSIS_QR_SMALL_BLOCK_DECODE		-17		// QRブロック(小さい方)解析失敗

#define RESULT_ERROR_ENCODE_YET				-21		// 未暗号処理
#define RESULT_ERROR_IMAGEFILE_WRITE		-22		// 画像書込失敗
#define RESULT_ERROR_SIZE_TO_SETTING		-23		// 設定とサイズの不整合
#define RESULT_ERROR_DECODE_FAILURE			-24		// 暗号化失敗
#define RESULT_ERROR_FILE_SAVE_FAILTURE		-25		// ファイル保存失敗
#define RESULT_ERROR_COLOR_TO_LAYER_SETTING	-26		// 色コードの設定エラー

// 画面表示用
// １〜２４までは、QRコードの層とします。
#define	VIEW_IMAGE_NONE						0		// 表示なし
#define VIEW_IMAGE_PM_CODE					100		// PMコード画像（補正前）
#define VIEW_IMAGE_CORRECT_PM_CODE			101		// PMコード画像（補正後）


// 定数宣言
#define MIN_LAYERSIZE		3					// PMコード最小層数
#define MAX_LAYERSIZE		24					// PMコード最大層数
#define MIN_VERSIONSIZE		1					// PMコード最小型数
#define MAX_VERSIONSIZE		40					// PMコード最大型数
#define MIN_MODULE			1					// 最小モジュールサイズ
#define MAX_MODULE			20					// 最大モジュールサイズ
#define MIN_MASK_NUMBER		0					// 最小マスク番号
#define MAX_MASK_NUMBER		7					// 最大マスク番号
#define MAX_MODULESIZE		177					// QRコードモジュール最大数
#define FUNCTION_CODE		'0x20'				// 機能コード
#define SYMBOL_COUNT		3					// シンボルマークの個数

#define VERSION_AUTO		0					// 型番：自動

#define INPUT_TYPE_FILE		0					// ファイルからの入力
#define INPUT_TYPE_CAMERA	1					// カメラ入力

#define PM_CODE_BIT_SIZE	3					// PMコードのビットサイズ（２４ビットカラーなので３となる）
#define PM_CODE_HEADER_LAYER_CODE		21
#define PM_CODE_HEADER_COLOR_CODE		20
#define PM_CODE_HEADER_COLOR_CODE2		44

#define EXT_SIZE			4					// 拡張子サイズ
#define	SIZE_SIZE			4					// 復号データサイズの使用サイズ（バイト）
#define RESERVE_SIZE		8					// 予備サイズ
#define HEADER_SIZE			EXT_SIZE + SIZE_SIZE + RESERVE_SIZE

#define BIT_ON				'\x01'				// ビットＯＮ（黒）
#define BIT_OFF				'\x00'				// ビットＯＦＦ（白）

#define QR_CODE_BIT_ON		'\x00'				// ビットＯＮ（黒）
#define QR_CODE_BIT_OFF		'\xFF'				// ビットＯＦＦ（白）
#define QR_CODE_BIT_OTHER	'\x88'				// その他ビット（三値化用）

#define READ_HORIZONTAL		0					// 横方向読込
#define READ_VERTICAL		1					// 縦方向読込

#define VERSION_SIZE		4
#define VERSION_CALC_FINDER	17

// 誤り訂正レベル
#define QR_LEVEL_L			0
#define QR_LEVEL_M			1
#define QR_LEVEL_Q			2
#define QR_LEVEL_H			3

// 閾値
#define THRESHOLD_SYMBOL_POSITON			3	// 左上位置補正マークの角度閾値（deg）

#define VER2SYM(ver)		(((ver) * VERSION_SIZE) + VERSION_CALC_FINDER)
#define SYM2VER(sym)		(((sym) - VERSION_CALC_FINDER) / VERSION_SIZE)


// 色定義
#define	BLACK								RGB ( 0, 0, 0 )	//	黒
#define	RED									RGB ( 255, 0, 0 )	//	赤
#define	GREEN								RGB ( 0, 255, 0 )	//	緑
#define	BLUE								RGB ( 0, 0, 255 )	//	青
#define	SYAN								RGB ( 0, 255, 255 )	//	シアン
#define	MAGENTA								RGB ( 255,   0, 255 )	//	マゼンタ
#define	YELLOW								RGB ( 255, 255,   0 )	//	黄色
#define	WHITE								RGB ( 255, 255, 255 )	//	白
#define	LIGHTBLUE							RGB ( 164, 255, 255 )	//	みずいろ
#define	LIGHTGREEN							RGB ( 181, 255, 172 )	//	薄い緑
#define	LIGHTYELLOW							RGB ( 255, 255, 196 )	//	薄い黄色
#define	LIGHTRED							RGB ( 254, 158, 189 )	//	薄い赤
#define	BROWN								RGB ( 250, 110,   6 )	//	茶色
#define	ORANGE								RGB ( 255,  90,   0 )	//	オレンジ
#define	NORMAL								RGB ( 192, 192, 192 )	//	通常のウインドウの色

#define PAI									3.14159265358979

// INIファイル用定義
// マスク用
#define INI_SECTION_MASK_PATTERN			"MaskPattern"
#define INI_KEY_MASK_LAYER					"MaskLayer"

// 色設定画面用
#define INI_SECTION_COLOR_CODE_DEFINITION	"ColorCodeDefinition"
#define INI_SECTION_COLOR_CODE3				"ColorCode3"
#define INI_SECTION_COLOR_CODE4TO9			"ColorCode4to9"
#define INI_SECTION_COLOR_CODE10To24		"ColorCode10to24"
#define INI_KEY_COLOR_LAYER					"ColorLayer"

#define INI_SECTION_COLOR_CODE_THRESHOLD	"Threshold"
#define INI_KEY_THREE_VALUE_THRESHOLD		"ThreeValue"
#define INI_KEY_COLOR_CODE_THRESHOLD		"ColorCode"
#define INI_KEY_CORRECT_SLOPE_THRESHOLD		"CorrectSlope"
#define INI_KEY_CORRECT_COLOR_THRESHOLD		"CorrectColor"

#define		FILETYPE_BMP	"BMP"
#define		FILETYPE_JPEG	"JPG"
#define		FILETYPE_GIF	"GIF"
#define		FILETYPE_PNG	"PNG"
#define		FILETYPE_PMC	"PMC"

// ファイル形式宣言
enum {
	FILE_TYPE_BMP = 0,														// BMP
	FILE_TYPE_JPEG,															// JPEG
	FILE_TYPE_GIF,															// GIF
	FILE_TYPE_PNG,															// PNG
	FILE_TYPE_PMC,															// PMP
	FILE_TYPE_NON_SUPORT													// 未サポート
};

// デバッグ用
#define INI_SECTION_DEBUG					"DebugCode"
#define INI_KEY_CHECK_FILE					"CheckFile"


#define COLORTABLE_SIZE						68
#endif
