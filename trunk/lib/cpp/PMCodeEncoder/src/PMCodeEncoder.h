/*
 * PMCodeEncoder.h
 *
 *  Created on: 2009/11/19
 *      Author: gabu
 */

#ifndef PMCODEENCODER_H_
#define PMCODEENCODER_H_

#include "define.h"
#include "Global.h"
#include "PMCodeEncode.h"
#include "PMCodeEncodeParameter.h"
#include "PMCodeImageData.h"

class PMCodeEncoder {
public:
	PMCodeEncoder();
	virtual ~PMCodeEncoder();
	int encode(PMCodeEncodeParameter param, PMCodeImageData &result);
	BOOL check(PMCodeEncodeParameter &param);

private:
	CPMCodeEncode mCPMCodeEncode;
};

#endif /* PMCODEENCODER_H_ */
