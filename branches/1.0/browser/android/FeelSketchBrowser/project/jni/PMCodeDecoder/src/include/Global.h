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

#ifndef GLOBAL_H
#define	GLOBAL_H

#include "define.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

long CalcPitch (short BitCount,long Width);


extern LONG			AscHexToLong(PSTR pszData, int ilength);
extern void			b1th2( unsigned char dat, char *p );
extern unsigned char h2tb1( char *p );

extern BOOL			MakeCheckFile(char * szFile, char * szImage, int iWidth, int iHeight, BOOL iStartTop);
extern void			MakeColorTable (int iColor, int iLayer);

extern char			g_CurrentPath [MAX_PATH];
extern int			g_MaskSetting [MAX_LAYERSIZE];
extern long			g_ColorCodeSettingDefinition [MAX_LAYERSIZE];
extern long			g_ColorCodeSetting3 [3];
extern long			g_ColorCodeSetting4to9 [9];
extern long			g_ColorCodeSetting10to24 [MAX_LAYERSIZE];
extern long			g_ColorCodeTable [COLORTABLE_SIZE + 1];
extern int			g_ColorCodeTableSize;
extern char			g_IniFilePath [MAX_PATH];
extern int			g_SelectMenuCode;
extern int			g_ColorCodeSetting;
extern int			g_ThreeValueThreshold;
extern int			g_ColorCodeThreshold;
extern int			g_CorrectSlopeThreshold;
extern int			g_CorrectColorThreshold;
extern BYTE			g_byModuleData [MAX_MODULESIZE][MAX_MODULESIZE];
extern long			g_PMCodeColor [COLORTABLE_SIZE + 1];
extern int			g_iLevelingRate;
extern int			g_iCheckFileOutput;

typedef struct tagRS_BLOCKINFO
{
	int ncRSBlock;
	int ncAllCodeWord;
	int ncDataCodeWord;

} RS_BLOCKINFO, *LPRS_BLOCKINFO;

typedef struct tagQR_VERSIONINFO
{
	int nVersionNo;
	int ncAllCodeWord;

	int ncDataCodeWord [4];

	int ncAlignPoint;
	int nAlignPoint [6];

	RS_BLOCKINFO RS_BlockInfo1 [4];
	RS_BLOCKINFO RS_BlockInfo2 [4];

} QR_VERSIONINFO, *LPQR_VERSIONINFO;

typedef struct tagPOINT {
   LONG x;
   LONG y;
} POINT;

extern QR_VERSIONINFO QR_VersonInfo [41];

extern char g_szLayerSetting [22][12];

#endif
