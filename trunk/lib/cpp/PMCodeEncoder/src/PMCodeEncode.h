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

#ifndef     __PM_CODE_ENCODE_H__
#define     __PM_CODE_ENCODE_H__

#include "define.h"
#include "QRCodeToPMCode.h"
#include "PMCodeMakeHeader.h"
#include "ImageUtil.h"

class CPMCodeEncode : public CQRCodeToPMCode, CPMCodeMakeHeader
{
public:
	CPMCodeEncode ();
	~CPMCodeEncode ();

	int		SetPMCodeData (char *szData, UINT uiDataSize, int iLayer, int iVersion, int iRSLevel, BOOL bMaskAuto, int iModuleSize);
	int 	SetPMCodeData (char *data, UINT data_size, int layer
						  , int version, int error_correction, BOOL use_mask, int module_size
						  , char *extension, char *note);
	int		EncodePMCode ();
	int		GetPMCodeImageData (char **szImage, UINT *uiImageSize, int *iWidth, int *iHeight);
	BOOL	CheckDataSizeToPMCode (UINT uiDataSize, int iLayer, int iVersion, int iRSLevel);

	UINT	GetQRCodeSize (int iLayer);
	int		GetQRCodeImageData (char **szImage, UINT *uiImageSize, int *iWidth, int *iHeight, int iLayer);

private:
	char		*m_szEncodeData;
	UINT		m_uiDataSize;
	char		*m_szPMCodeImage;
	UINT		m_uiPMCodeImageSize;
	char		*m_szQRCodeImage [MAX_LAYERSIZE];
	UINT		m_uiQRCodeImageSize [MAX_LAYERSIZE];
	int			m_iWidth;
	int			m_iHeight;
	int			m_iLayer;
	int			m_iVersion;
	int			m_iSymbolSize;
	int			m_iRSLevel;
	BOOL		m_bMaskAuto;
	int			m_iModuleSize;
};
#endif
