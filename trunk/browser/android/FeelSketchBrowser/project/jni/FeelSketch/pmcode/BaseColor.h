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

#ifndef BASECOLOR_H_
#define BASECOLOR_H_

#include <jni.h>
#include <string.h>

namespace pmcode {
class BaseColor {
public:
	BaseColor(JNIEnv*, jobject);
	virtual ~BaseColor();
	int getColorOf3Layers(int);
	int getColorOf4Layers(int);
	void getColorsOf3Layers(int*);
	void getColorsOf4Layers(int*);
private:
	int mColorsOf3Layers[3];
	int mColorsOf4Layers[4];
};
}

#endif /* BASECOLOR_H_ */
