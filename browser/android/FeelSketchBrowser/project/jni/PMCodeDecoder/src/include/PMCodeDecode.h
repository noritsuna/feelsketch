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

#ifndef     __PM_CODE_DECODE_H__
#define     __PM_CODE_DECODE_H__

#include "PMCodeAnalysis.h"
#include "PMCodeToQRCodes.h"

class CPMCodeDecode : public CPMCodeAnalysis, CPMCodeToQRCodes
{
public:
	CPMCodeDecode ();
	~CPMCodeDecode ();

	void	SetBaseColor (int *baseColors, int layer);
	int		SetBaseImage (char *szImage, UINT uiImageSize, int iWidth, int iHeight);
	int		DecodePMCode ();
	int		GetPMCodeDecodeData (char *szData, UINT uiBufferSize);

	UINT	GetQRCodeSize (int iLayer);
	int		GetQRCodeImage (char **szImage, UINT *uiImageSize, int *iWidth, int *iHeight, int iLayer);

private:
	char		*m_szBaseImage;
	UINT		m_uiBaseImageSize;
	char		*m_szQRCodeImage;
	UINT		m_uiQRCodeImageSize;
	char		*m_szDecodeData;
	UINT		m_uiDecodeDataSize;
	int			m_Width;
	int			m_Height;
	int			m_iLayer;
	int			m_iVersion;
	int			m_iSymbolSize;
	int			m_iRSLevel;
	int			m_iMaskPattern [MAX_LAYERSIZE];	
};


#endif
