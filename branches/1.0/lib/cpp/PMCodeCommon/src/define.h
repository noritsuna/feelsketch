/**
***                  "Feel Sketch" PMCode Encoder & Decoder.
***    Copyright (C) 2009, Content Idea of ASIA Co.,Ltd. (oss.pmcode@ci-a.com)
***
***    This program is free software: you can redistribute it and/or modify
***    it under the terms of the GNU General Public License as published by
***    the Free Software Foundation, either version 3 of the License, or
***    (at your option) any later version.
***
***    This program is distributed in the hope that it will be useful,
***    but WITHOUT ANY WARRANTY; without even the implied warranty of
***    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
***    GNU General Public License for more details.
***
***    You should have received a copy of the GNU General Public License
***    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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


#define BIT_COUNT_8						8
#define BIT_COUNT_16					16
#define BIT_COUNT_24					24
#define BIT_COUNT_32					32

#define	LEVELING_TYPE_CENTER			0
#define LEVELING_TYPE_CENTER_VICINITY	1
#define	LEVELING_TYPE_ALL				2




#define PM_CODE_COLOR						1
#define	INI_FILE							"PMCode.INI"

#define RESULT_OK							0
#define RESULT_ERROR_SECURE_MEMORY			-1
#define RESULT_ERROR_DECODE_YET				-2
#define RESULT_ERROR_IMAGEFILE_READ			-3
#define RESULT_ERROR_NON_SUPORT_FILE		-4
#define RESULT_ERROR_NON_SUPORT_COLOR		-5
#define RESULT_ERROR_IMAGEFILE_READ_YET		-6
#define RESULT_ERROR_MARK_UNDETECTED		-7
#define RESULT_ERROR_QR_CODE_HEADER			-8
#define RESULT_ERROR_COLLECT_FAILTURE		-9
#define RESLUT_ERROR_LAYER_RESOLUTION		-10
#define RESULT_ERROR_ANALYSIS_FAILURE		-11
#define RESULT_ERROR_ANALYSIS_LAYER			-12
#define RESULT_ERROR_ANALYSIS_QR_IMAGE		-13
#define RESULT_ERROR_ANALYSIS_QR_STATUS		-14
#define RESULT_ERROR_ANALYSIS_QR_DECODE		-15
#define RESULT_ERROR_ANALYSIS_QR_BIG_BLOCK_DECODE		-16
#define RESULT_ERROR_ANALYSIS_QR_SMALL_BLOCK_DECODE		-17

#define RESULT_ERROR_ENCODE_YET				-21
#define RESULT_ERROR_IMAGEFILE_WRITE		-22
#define RESULT_ERROR_SIZE_TO_SETTING		-23
#define RESULT_ERROR_DECODE_FAILURE			-24
#define RESULT_ERROR_FILE_SAVE_FAILTURE		-25
#define RESULT_ERROR_COLOR_TO_LAYER_SETTING	-26

#define	VIEW_IMAGE_NONE						0
#define VIEW_IMAGE_PM_CODE					100
#define VIEW_IMAGE_CORRECT_PM_CODE			101


#define MIN_LAYERSIZE		3
#define MAX_LAYERSIZE		24
#define MIN_VERSIONSIZE		1
#define MAX_VERSIONSIZE		40
#define MIN_MODULE			1
#define MAX_MODULE			20
#define MIN_MASK_NUMBER		0
#define MAX_MASK_NUMBER		7
#define MAX_MODULESIZE		177
#define FUNCTION_CODE		'0x20'
#define SYMBOL_COUNT		3

#define VERSION_AUTO		0

#define INPUT_TYPE_FILE		0
#define INPUT_TYPE_CAMERA	1

#define PM_CODE_BIT_SIZE	3
#define PM_CODE_HEADER_LAYER_CODE		21
#define PM_CODE_HEADER_COLOR_CODE		20
#define PM_CODE_HEADER_COLOR_CODE2		44

#define EXT_SIZE			4
#define	SIZE_SIZE			4
#define RESERVE_SIZE		8
#define HEADER_SIZE			EXT_SIZE + SIZE_SIZE + RESERVE_SIZE

#define BIT_ON				'\x01'
#define BIT_OFF				'\x00'

#define QR_CODE_BIT_ON		'\x00'
#define QR_CODE_BIT_OFF		'\xFF'
#define QR_CODE_BIT_OTHER	'\x88'

#define READ_HORIZONTAL		0
#define READ_VERTICAL		1

#define VERSION_SIZE		4
#define VERSION_CALC_FINDER	17

#define QR_LEVEL_L			0
#define QR_LEVEL_M			1
#define QR_LEVEL_Q			2
#define QR_LEVEL_H			3

#define THRESHOLD_SYMBOL_POSITON

#define VER2SYM(ver)		(((ver) * VERSION_SIZE) + VERSION_CALC_FINDER)
#define SYM2VER(sym)		(((sym) - VERSION_CALC_FINDER) / VERSION_SIZE)


#define	BLACK								RGB ( 0, 0, 0 )
#define	RED									RGB ( 255, 0, 0 )
#define	GREEN								RGB ( 0, 255, 0 )
#define	BLUE								RGB ( 0, 0, 255 )
#define	SYAN								RGB ( 0, 255, 255 )
#define	MAGENTA								RGB ( 255,   0, 255 )
#define	YELLOW								RGB ( 255, 255,   0 )
#define	WHITE								RGB ( 255, 255, 255 )
#define	LIGHTBLUE							RGB ( 164, 255, 255 )
#define	LIGHTGREEN							RGB ( 181, 255, 172 )
#define	LIGHTYELLOW							RGB ( 255, 255, 196 )
#define	LIGHTRED							RGB ( 254, 158, 189 )
#define	BROWN								RGB ( 250, 110,   6 )
#define	ORANGE								RGB ( 255,  90,   0 )
#define	NORMAL								RGB ( 192, 192, 192 )

#define PAI									3.14159265358979

#define INI_SECTION_MASK_PATTERN			"MaskPattern"
#define INI_KEY_MASK_LAYER					"MaskLayer"

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

enum {
	FILE_TYPE_BMP = 0,
	FILE_TYPE_JPEG,
	FILE_TYPE_GIF,
	FILE_TYPE_PNG,
	FILE_TYPE_PMC,
	FILE_TYPE_NON_SUPORT
};

#define INI_SECTION_DEBUG					"DebugCode"
#define INI_KEY_CHECK_FILE					"CheckFile"


#define COLORTABLE_SIZE						68
#endif
