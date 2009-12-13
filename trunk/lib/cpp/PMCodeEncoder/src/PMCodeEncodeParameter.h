/*
 * PMCodeEncodeParameter.h
 *
 *  Created on: 2009/11/18
 *      Author: gabu
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
