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

#include "cv.h"
#include "highgui.h"
#include <jni.h>

#include "util.h"
#include "Decoder.h"
#include "Detector.h"
#include "GrayBytesMonochromeBitmapSource.h"
#include "BaseColor.h"
#include "OutputColor.h"
#include "ColorDetector.h"
#include "Drawer.h"
#include "Parser.h"
#include "RectCalculator.h"


#ifdef __cplusplus
extern "C" {
#endif

jint Java_jp_co_cia_feelsketch_DecodePMCodeThread_getPMResult( JNIEnv*  env,
                                    jobject  thiz,
									jintArray src,
									jint width,
									jint height,
									jint left,
									jint top,
									jint size,
									jobject pmResult,
									jobject colorSettings);

jintArray Java_jp_co_cia_feelsketch_Preview_getRect( JNIEnv*  env,
                                    jobject  thiz,
									jintArray src,
									jint width,
									jint height,
									jint pmCodeSize,
									jint pmCodePos,
									jint paperSize);

void Java_jp_co_cia_feelsketch_Preview_getFSResult( JNIEnv* env,
									jobject thiz,
									jbyteArray fsData,
									jint fsDataSize,
									jobject jfsResult,
									jint imageWidth,
									jint imageHeight);

#ifdef __cplusplus
}
#endif

