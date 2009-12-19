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

#ifndef     __QR_CODE_MASK_H__
#define     __QR_CODE_MASK_H__

#include "define.h"

class CQRCodeMask
{
public:
	CQRCodeMask ();
	~CQRCodeMask ();

	void GetMaskQRCode (char * szImage, UINT iImageSize, int iMaskPattern
												, int iVersion);
	void SetMaskQRCode (int iMaskPattern, int iVersion);
};
#endif

