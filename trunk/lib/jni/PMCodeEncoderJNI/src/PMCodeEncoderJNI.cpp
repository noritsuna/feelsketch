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
	param.setData(src);				// データをセット
	param.setDataSize(env->GetArrayLength(data));	// データのサイズ(byte数)をセット

	// エンコード！
	int ret = 0;
	ret = encoder.encode(param, imageData);
	if (ret != RESULT_OK) return ret;

	// 結果を格納
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
