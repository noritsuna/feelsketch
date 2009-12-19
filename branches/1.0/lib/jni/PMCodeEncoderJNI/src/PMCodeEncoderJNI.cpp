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

#include "PMCodeEncoderJNI.h"

#include "Global.h"
#include "PMCodeEncoder.h"
#include "PMCodeEncodeParameter.h"
#include "PMCodeImageData.h"

JNIEXPORT jint JNICALL Java_jp_co_cia_pmcode_PMCodeEncoderJNI_encode(JNIEnv *env, jobject thiz, jbyteArray data, jobject result) {

	PMCodeEncoder encoder;
	PMCodeEncodeParameter param;
	PMCodeImageData imageData;

	char *src = (char*)env->GetByteArrayElements(data, NULL);
	param.setData(src);
	param.setDataSize(env->GetArrayLength(data));

	int ret = 0;
	ret = encoder.encode(param, imageData);
	if (ret != RESULT_OK) return ret;

  	jclass clazz = env->GetObjectClass(result);
 	jmethodID methodID = env->GetMethodID(clazz, "setWidth", "(I)V");
  	env->CallVoidMethod(result, methodID, imageData.getWidth());
 	methodID = env->GetMethodID(clazz, "setHeight", "(I)V");
  	env->CallVoidMethod(result, methodID, imageData.getHeight());
 	methodID = env->GetMethodID(clazz, "setImage", "([B)V");
 	int len = imageData.getImageSize();
 	char *image = imageData.getImage();
 	jbyteArray dstArray = env->NewByteArray(len);
 	env->SetByteArrayRegion(dstArray, 0, len, (jbyte*)image);
 	env->CallVoidMethod(result, methodID, dstArray);

	return RESULT_OK;
}
