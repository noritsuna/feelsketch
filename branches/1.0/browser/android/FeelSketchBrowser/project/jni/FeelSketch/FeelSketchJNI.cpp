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

#include "FeelSketchJNI.h"
#include <stdio.h>

using namespace pmcode;
using namespace feelsketch;

IplImage* loadPixels(int* pixels, int width, int height)
{
	int x, y;
	IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

	for ( y = 0; y < height; y++ ) {
        for ( x = 0; x < width; x++ ) {
            // blue
            IMAGE( img, x, y, 0 ) = pixels[x+y*width] & 0xFF;
            // green
            IMAGE( img, x, y, 1 ) = pixels[x+y*width] >> 8 & 0xFF;
            // red
            IMAGE( img, x, y, 2 ) = pixels[x+y*width] >> 16 & 0xFF;
        }
    }

	return img;
}

int Java_jp_co_cia_feelsketchbrowser_DecodePMCodeThread_getPMResult( JNIEnv*  env,
                                    jobject  thiz,
									jintArray src,
									jint width,
									jint height,
									jint left,
									jint top,
									jint size,
									jobject pmResult,
									jobject colorSettings)
{
	LOG_FUNC_START;
  	jboolean b;
  	int x, y, i, j, error;
  	int* pixels = (env)->GetIntArrayElements(src, &b);
	char result_data[4096];
	memset (result_data, '\0', 4096);
	char result_ext[10] = "";
	char result_reserve[16] = "";
	int result_data_size = 0;
 	char msg[128];
	
	BaseColor baseColor = BaseColor(env, colorSettings);
	OutputColor outputColors = OutputColor(baseColor);

	IplImage *load_image = loadPixels(pixels, width, height);
	if(load_image == NULL)
	{
		return -1;
	}

    // clip rectangle
    cvSetImageROI(load_image, cvRect(left, top, size, size));
    CvSize image_size = cvGetSize(load_image);
    IplImage *source_image = cvCreateImage(image_size, IPL_DEPTH_8U, 3);
	CvScalar color;
    for (y = 0; y < image_size.height; y++) {
        for (x = 0; x < image_size.width; x++) {
			color = cvGet2D(load_image, y, x);
			cvSet2D(source_image, y, x, color);
        }
   	}

	IplImage *grey_image = cvCreateImage(image_size, IPL_DEPTH_8U, 1);
    cvCvtColor(source_image, grey_image, CV_BGR2GRAY);
	// reverse white to black for zxing
    for (y = 0; y < image_size.height; y++) {
        for (x = 0; x < image_size.width; x++) {
			color = cvGet2D(grey_image, y, x);
			color.val[0] = 255 - color.val[0];
			cvSet2D(grey_image, y, x, color);
        }
   	}

    // detect module coordinate by zxing
	Ref<MonochromeBitmapSource> imageSrc(new GrayBytesMonochromeBitmapSource((unsigned char *)grey_image->imageData, grey_image->width, grey_image->height, grey_image->widthStep));
	qrcode::detector::Detector detector(imageSrc);
	if ((error = detector.detect()) == 0) {
		Ref<DetectorResult> detectorResult(detector.getResult());
		Ref<PointMatrix> points(detectorResult->getPoints());
		int dimension = points->getDimension();
		int size = ((dimension*3) + 1) * dimension;
		unsigned char *data = (unsigned char*)malloc(sizeof(unsigned char)*size);

		// detect module color
		ColorDetector colorDetector(source_image, points, outputColors);
		colorDetector.detect(data);
		int layer_count = colorDetector.getLayerNum();
		int base_color[24] = {0};
		if (layer_count == 3) {
			baseColor.getColorsOf3Layers(base_color);
		} else if (layer_count == 4) {
			baseColor.getColorsOf4Layers(base_color);
		}

		Decoder decoder;
		error = decoder.decode(data, size, dimension, result_data, result_ext, result_reserve, &result_data_size, base_color, layer_count);
		sprintf(msg, "decode result = %d", error);
		LOG_FUNC_MESSAGE(msg);
		if (error != 0) {
			sprintf(result_data, "decode error.\nerror code: %d", error);
			result_data_size = strlen(result_data);
		}
		free(data);
	} else {
		sprintf(result_data, "detect error.");
		result_data_size = strlen(result_data);
	}
    
    cvReleaseImage(& load_image);
    cvReleaseImage(& source_image);

	(env)->ReleaseIntArrayElements(src, pixels, 0);

	// store result data
	jbyteArray byte_data = (env)->NewByteArray(result_data_size);
	(env)->SetByteArrayRegion(byte_data, 0, result_data_size, (jbyte*)result_data);
  	jclass pmResultClass = env->GetObjectClass(pmResult);
  	jfieldID fieldID = env->GetFieldID(pmResultClass, "data", "[B");
  	env->SetObjectField(pmResult, fieldID, (jbyteArray)byte_data);

	jstring ext_str = (env)->NewStringUTF(result_ext);
	fieldID = env->GetFieldID(pmResultClass, "extension", "Ljava/lang/String;");
  	env->SetObjectField(pmResult, fieldID, (jobject)ext_str);

	jstring reserve_str = (env)->NewStringUTF(result_reserve);
	fieldID = env->GetFieldID(pmResultClass, "title", "Ljava/lang/String;");
  	env->SetObjectField(pmResult, fieldID, (jobject)reserve_str);

  	return error;
}

jintArray Java_jp_co_cia_feelsketchbrowser_Preview_getRect( JNIEnv*  env,
                                    jobject  thiz,
									jintArray src,
									jint width,
									jint height,
									jint pmCodeSize,
									jint pmCodePos,
									jint paperSize)
{
  LOG_FUNC_START;
	jboolean b;
  	int x, y, i, j;
  	int* pixels = (env)->GetIntArrayElements(src, &b);
  	int pmCodeRect[4], resultRect[4];
 	char msg[128];
 	CvScalar color;

	IplImage *image = loadPixels(pixels, width, height);
    CvSize imageSize = cvGetSize(image);
	IplImage *greyImage = cvCreateImage(imageSize, IPL_DEPTH_8U, 1);
    cvCvtColor(image, greyImage, CV_BGR2GRAY);
	// reverse white to black for zxing
    for (y = 0; y < imageSize.height; y++) {
        for (x = 0; x < imageSize.width; x++) {
			color = cvGet2D(greyImage, y, x);
			color.val[0] = 255 - color.val[0];
			cvSet2D(greyImage, y, x, color);
        }
   	}

    // detect module coordinate by zxing
	Ref<MonochromeBitmapSource> imageSrc(new GrayBytesMonochromeBitmapSource((unsigned char *)greyImage->imageData, greyImage->width, greyImage->height, greyImage->widthStep));
	qrcode::detector::Detector detector(imageSrc);
	if (detector.detect() == 0) {
		Ref<DetectorResult> detectorResult(detector.getResult());
		Ref<PointMatrix> points(detectorResult->getPoints());
		int dimension = points->getDimension();
		pmCodeRect[0] = points->get(0, 0)->getX();
		pmCodeRect[1] = points->get(0, 0)->getY();
		pmCodeRect[2] = points->get(dimension-1, dimension-1)->getX();
		pmCodeRect[3] = points->get(dimension-1, dimension-1)->getY();

		RectCalculator calculator;
		calculator.calculate(resultRect, pmCodeRect, pmCodeSize, (RectCalculator::PMCodePos)pmCodePos, paperSize, width, height);
	} else {
		resultRect[0] = 0;
		resultRect[1] = 0;
		resultRect[2] = 0;
		resultRect[3] = 0;
	}

	jintArray result = (env)->NewIntArray(4);
	(env)->SetIntArrayRegion(result, 0, 4, resultRect);

    cvReleaseImage(& image);
    cvReleaseImage(& greyImage);

	(env)->ReleaseIntArrayElements(src, pixels, 0);

	return result;
}

void Java_jp_co_cia_feelsketchbrowser_Preview_getFSResult( JNIEnv* env,
									jobject thiz,
									jbyteArray fsData,
									jint fsDataSize,
									jobject jfsResult,
									jint imageWidth,
									jint imageHeight)
{
	LOG_FUNC_START;
	jboolean b;
  	int x, y, i, j;
  	int length = imageWidth*imageHeight;
  	char* data = (char*)(env)->GetByteArrayElements(fsData, &b);
 	char msg[128];

 	Parser parser(data, fsDataSize, imageWidth, imageHeight);
 	FSResult* fsResult = new FSResult();
 	int error = parser.parse(fsResult);
	sprintf(msg, "parse result = %d", error);
	LOG_FUNC_MESSAGE(msg);
	sprintf(msg, "pmcode pos = %d, pmcode size = %d, paper size = %d", fsResult->pmcodePos, fsResult->pmcodeSize, fsResult->paperSize);
	LOG_FUNC_MESSAGE(msg);

	if (error < 0) return;

	jfieldID fieldID;

	jintArray imageData = (env)->NewIntArray(length);
	(env)->SetIntArrayRegion(imageData, 0, length, fsResult->imageData);
  	jclass jfsResultClass = env->GetObjectClass(jfsResult);
  	fieldID = env->GetFieldID(jfsResultClass, "imageData", "[I");
  	env->SetObjectField(jfsResult, fieldID, imageData);

  	fieldID = env->GetFieldID(jfsResultClass, "pmcodePos", "I");
  	env->SetIntField(jfsResult, fieldID, (jint)fsResult->pmcodePos);

  	fieldID = env->GetFieldID(jfsResultClass, "pmcodeSize", "I");
  	env->SetIntField(jfsResult, fieldID, (jint)fsResult->pmcodeSize);

  	fieldID = env->GetFieldID(jfsResultClass, "paperSize", "I");
  	env->SetIntField(jfsResult, fieldID, (jint)fsResult->paperSize);

}
