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

#ifndef     __PM_CODE_MAKE_HEADER_H__
#define     __PM_CODE_MAKE_HEADER_H__

#define LAYER_CODE_BUF_SIZE				12

#define LAYER_BIT						4
#define LAYER_CODE_POS_X				5
#define COLOR_CODE_POS_X				4

#include "define.h"

class CPMCodeMakeHeader
{
public:
	CPMCodeMakeHeader ();
	~CPMCodeMakeHeader ();

	int SetPMCodeHeader (char *szImage, UINT iImageSize, int iSymbolSize, int iLayer);			

private:
	int			m_iLayer;
	int			m_iVersion;
	int			m_iSymbolSize;
	int			m_iRSLevel;

};
#endif