/*
 * PMCodeColorCodeSetting.h
 *
 *  Created on: 2009/11/18
 *      Author: gabu
 */

#ifndef PMCODECOLORCODESETTING_H_
#define PMCODECOLORCODESETTING_H_

#include "define.h"

class PMCodeColorCodeSetting {
public:
	PMCodeColorCodeSetting();
	virtual ~PMCodeColorCodeSetting();
	void setColorCode(int layer, long value);
	long getColorCode(int layer);

private:
	long mColorCodes[MAX_LAYERSIZE];
};

#endif /* PMCODECOLORCODESETTING_H_ */
