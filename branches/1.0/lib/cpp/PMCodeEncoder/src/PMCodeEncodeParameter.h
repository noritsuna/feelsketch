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

#ifndef PMCODEENCODEPARAMETER_H_
#define PMCODEENCODEPARAMETER_H_

#include "PMCodeColorCodeSetting.h"
#include <stddef.h>

class PMCodeEncodeParameter {
public:
	PMCodeEncodeParameter();
	virtual ~PMCodeEncodeParameter();

	// Setter/Getter
	void setPMCodeColorCodeSetting(PMCodeColorCodeSetting pmCodeColorCodeSetting);
	PMCodeColorCodeSetting getPMCodeColorCodeSetting();
	void setData(char *data);
	char* getData();
	void setDataSize(UINT dataSize);
	UINT getDataSize();
	void setLayer(int layer);
	int getLayer();
	void setVersion(int version);
	int getVersion();
	void setErrorCorrection(int errorCorrection);
	int getErrorCorrection();
	void setMaskAuto(BOOL maskAuto);
	BOOL getMaskAuto();
	void setModuleSize(int moduleSize);
	int getModuleSize();
	void setExtension(char *extension);
	char* getExtension();
	void setNote(char *note);
	char* getNote();

private:
	PMCodeColorCodeSetting	mPMCodeColorCodeSetting;
	char 					*mData;
	UINT					mDataSize;
	int						mLayer;
	int						mVersion;
	int						mErrorCorrection;
	BOOL					mMaskAuto;
	int						mModuleSize;
	char					*mExtension;
	char					*mNote;
};

#endif /* PMCODEENCODEPARAMETER_H_ */
