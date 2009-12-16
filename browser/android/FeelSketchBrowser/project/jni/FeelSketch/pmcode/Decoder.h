/**
***                          "Feel Sketch" browser.
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

#ifndef DECODER_H_
#define DECODER_H_

#include "global.h"
#include "PMCodeReader.h"

namespace pmcode {
class Decoder {
public:
	Decoder();
	virtual ~Decoder();
	int decode(unsigned char* imageData, int dataSize, int dimension, char* result_text, char* result_ext, char* result_reserve, int* result_data_size, int *baseColors, int layer);
};
}

#endif /* DECODER_H_ */
