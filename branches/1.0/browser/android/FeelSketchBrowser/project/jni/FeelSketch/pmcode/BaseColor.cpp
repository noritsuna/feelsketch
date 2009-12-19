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

#include "BaseColor.h"

namespace pmcode {
BaseColor::BaseColor(JNIEnv *env, jobject colorSettings) {
	jclass clazz = env->GetObjectClass(colorSettings);
	jmethodID methodID = env->GetMethodID(clazz, "get3Layers", "()[I");
	jintArray intArray = (jintArray)env->CallObjectMethod(colorSettings, methodID);
	env->GetIntArrayRegion(intArray, 0, 3, mColorsOf3Layers);

	methodID = env->GetMethodID(clazz, "get4Layers", "()[I");
	intArray = (jintArray)env->CallObjectMethod(colorSettings, methodID);
	env->GetIntArrayRegion(intArray, 0, 4, mColorsOf4Layers);
}

BaseColor::~BaseColor() {
}

int BaseColor::getColorOf3Layers(int index) {
	return mColorsOf3Layers[index];
}

int BaseColor::getColorOf4Layers(int index) {
	return mColorsOf4Layers[index];
}

void BaseColor::getColorsOf3Layers(int* colors) {
	memcpy(colors, mColorsOf3Layers, sizeof(mColorsOf3Layers));
}

void BaseColor::getColorsOf4Layers(int* colors) {
	memcpy(colors, mColorsOf4Layers, sizeof(mColorsOf4Layers));
}
}
