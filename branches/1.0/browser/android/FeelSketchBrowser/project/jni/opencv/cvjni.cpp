/*
OpenCV for Android NDK
Copyright (c) 2006-2009 SIProp Project http://www.siprop.org/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#include "cvjni.h"


#define THRESHOLD	10
#define THRESHOLD_MAX_VALUE	255

#define CONTOUR_MAX_LEVEL	1
#define LINE_THICKNESS	2
#define LINE_TYPE	8


JNIEXPORT
jbooleanArray
JNICALL
Java_org_siprop_opencv_OpenCV_findContours(JNIEnv* env,
										jobject thiz,
										jintArray photo_data,
										jint width,
										jint height) {

	// Load Image
	IplImage *sourceImage;
	int* pixels = env->GetIntArrayElements(photo_data, NULL);
	sourceImage = loadPixels(pixels, width, height);
	if(sourceImage == NULL) {
		LOGV("Error loadPixels.");
		return NULL;
	}


	IplImage *grayImage = cvCreateImage( cvGetSize(sourceImage), IPL_DEPTH_8U, 1 );		//	�O���[�X�P�[���摜�pIplImage
	IplImage *binaryImage = cvCreateImage( cvGetSize(sourceImage), IPL_DEPTH_8U, 1 );	//	2�l�摜�pIplImage
	IplImage *contourImage = cvCreateImage( cvGetSize(sourceImage), IPL_DEPTH_8U, 3 );	//	�֊s�摜�pIplImage

	//	BGR����O���[�X�P�[���ɕϊ�����
	cvCvtColor( sourceImage, grayImage, CV_BGR2GRAY );

	//	�O���[�X�P�[������2�l�ɕϊ�����
	cvThreshold( grayImage, binaryImage, THRESHOLD, THRESHOLD_MAX_VALUE, CV_THRESH_BINARY );

	//	�֊s���o�p�̃��������m�ۂ���
	CvMemStorage* storage = cvCreateMemStorage( 0 );	//	���o���ꂽ�֊s��ۑ�����̈�
	CvSeq* find_contour = NULL;		//	�֊s�ւ̃|�C���^           

	//	2�l�摜���̗֊s�������A���̐���Ԃ�
	int find_contour_num = cvFindContours( 
		binaryImage,			//	���͉摜(�W�r�b�g�V���O���`�����l���j
		storage,				//	���o���ꂽ�֊s��ۑ�����̈�
		&find_contour,			//	��ԊO���̗֊s�ւ̃|�C���^�ւ̃|�C���^
		sizeof( CvContour ),	//	�V�[�P���X�w�b�_�̃T�C�Y
		CV_RETR_LIST,			//	���o���[�h 
		CV_CHAIN_APPROX_NONE,	//	�����@
		cvPoint( 0, 0 )			//	�I�t�Z�b�g
	);

	//	���̗̂֊s��ԐF�ŕ`�悷��
	CvScalar red = CV_RGB( 255, 0, 0 );
	cvDrawContours( 
		sourceImage,			//	�֊s��`�悷��摜
		find_contour,			//	�ŏ��̗֊s�ւ̃|�C���^
		red,					//	�O���֊s���̐F
		red,					//	�����֊s���i���j�̐F
		CONTOUR_MAX_LEVEL,		//	�`�悳���֊s�̍ő僌�x��
		LINE_THICKNESS,			//	�`�悳���֊s���̑���
		LINE_TYPE,				//	���̎��
		cvPoint( 0, 0 )			//	�I�t�Z�b�g
	);   


	jbooleanArray res_array;
	int imageSize;

	CvMat stub, *mat_image;
    int channels, ipl_depth;
    mat_image = cvGetMat( sourceImage, &stub );
    channels = CV_MAT_CN( mat_image->type );

    ipl_depth = cvCvToIplDepth(mat_image->type);

	WLNonFileByteStream* m_strm = new WLNonFileByteStream();
    loadImageBytes(mat_image->data.ptr, mat_image->step, mat_image->width,
                             mat_image->height, ipl_depth, channels, m_strm);
	LOGV("Load loadImageBytes.");


	imageSize = m_strm->GetSize();
	res_array = env->NewBooleanArray(imageSize);
	LOGV("Load NewByteArray.");
    if (res_array == NULL) {
        return NULL;
    }
    env->SetBooleanArrayRegion(res_array, 0, imageSize, (jboolean*)m_strm->GetByte());
	LOGV("Load SetBooleanArrayRegion.");



	cvReleaseImage( &sourceImage );
	cvReleaseImage( &binaryImage );
	cvReleaseImage( &grayImage );
	m_strm->Close();
	SAFE_DELETE(m_strm);


	return res_array;

}


#if 0

JNIEXPORT
jbooleanArray
JNICALL
Java_org_siprop_opencv_OpenCV_faceDetect(JNIEnv* env,
										jobject thiz,
										jintArray photo_data1,
										jintArray photo_data2,
										jint width,
										jint height) {
	LOGV("Load desp.");

	int i, x, y;
	int* pixels;
	IplImage *frameImage;
	
	IplImage *backgroundImage = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	IplImage *grayImage = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	IplImage *differenceImage = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	
	IplImage *hsvImage = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 3 );
	IplImage *hueImage = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	IplImage *saturationImage = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	IplImage *valueImage = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	IplImage *thresholdImage1 = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	IplImage *thresholdImage2 = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	IplImage *thresholdImage3 = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	IplImage *faceImage = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	
	CvMoments moment;
	double m_00;
	double m_10;
	double m_01;
	int gravityX;
	int gravityY;

	jbooleanArray res_array;
	int imageSize;



	// Load Image
	pixels = env->GetIntArrayElements(photo_data1, NULL);
	frameImage = loadPixels(pixels, width, height);
	if(frameImage == NULL) {
		LOGV("Error loadPixels.");
		return NULL;
	}
	
	
	cvCvtColor( frameImage, backgroundImage, CV_BGR2GRAY );
	
	
	pixels = env->GetIntArrayElements(photo_data2, NULL);
	frameImage = loadPixels(pixels, width, height);
	if(frameImage == NULL) {
		LOGV("Error loadPixels.");
		return NULL;
	}
	cvCvtColor( frameImage, grayImage, CV_BGR2GRAY );
	cvAbsDiff( grayImage, backgroundImage, differenceImage );
	
	cvCvtColor( frameImage, hsvImage, CV_BGR2HSV );
	LOGV("Load cvCvtColor.");
	cvSplit( hsvImage, hueImage, saturationImage, valueImage, NULL );
	LOGV("Load cvSplit.");
	cvThreshold( hueImage, thresholdImage1, THRESH_BOTTOM, THRESHOLD_MAX_VALUE, CV_THRESH_BINARY );
	cvThreshold( hueImage, thresholdImage2, THRESH_TOP, THRESHOLD_MAX_VALUE, CV_THRESH_BINARY_INV );
	cvAnd( thresholdImage1, thresholdImage2, thresholdImage3, NULL );
	LOGV("Load cvAnd.");
	
	cvAnd( differenceImage, thresholdImage3, faceImage, NULL );
	
	cvMoments( faceImage, &moment, 0 );
	m_00 = cvGetSpatialMoment( &moment, 0, 0 );
	m_10 = cvGetSpatialMoment( &moment, 1, 0 );
	m_01 = cvGetSpatialMoment( &moment, 0, 1 );
	gravityX = m_10 / m_00;
	gravityY = m_01 / m_00;
	LOGV("Load cvMoments.");


	cvCircle( frameImage, cvPoint( gravityX, gravityY ), CIRCLE_RADIUS,
		 CV_RGB( 255, 0, 0 ), LINE_THICKNESS, LINE_TYPE, 0 );




	CvMat stub, *mat_image;
    int channels, ipl_depth;
    mat_image = cvGetMat( frameImage, &stub );
    channels = CV_MAT_CN( mat_image->type );

    ipl_depth = cvCvToIplDepth(mat_image->type);

	WLNonFileByteStream* m_strm = new WLNonFileByteStream();
    loadImageBytes(mat_image->data.ptr, mat_image->step, mat_image->width,
                             mat_image->height, ipl_depth, channels, m_strm);
	LOGV("Load loadImageBytes.");


	imageSize = m_strm->GetSize();
	res_array = env->NewBooleanArray(imageSize);
	LOGV("Load NewByteArray.");
    if (res_array == NULL) {
        return NULL;
    }
    env->SetBooleanArrayRegion(res_array, 0, imageSize, (jboolean*)m_strm->GetByte());
	LOGV("Load SetBooleanArrayRegion.");




	cvReleaseImage( &backgroundImage );
	cvReleaseImage( &grayImage );
	cvReleaseImage( &differenceImage );
	cvReleaseImage( &hsvImage );
	cvReleaseImage( &hueImage );
	cvReleaseImage( &saturationImage );
	cvReleaseImage( &valueImage );
	cvReleaseImage( &thresholdImage1 );
	cvReleaseImage( &thresholdImage2 );
	cvReleaseImage( &thresholdImage3 );
	cvReleaseImage( &faceImage );
	cvReleaseImage( &frameImage );
	m_strm->Close();
	SAFE_DELETE(m_strm);

	return res_array;

}
#endif

