LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

STLPORT_BASE := $(LOCAL_PATH)/../stlport

LOCAL_MODULE    := ZxingDetector
LOCAL_C_INCLUDES := \
        $(LOCAL_PATH)/detector \
        $(LOCAL_PATH)/common \
		$(STLPORT_BASE)/stlport
LOCAL_CFLAGS += $(LOCAL_C_INCLUDES:%=-I%) \
     			-D__NEW__ \
     			-D__SGI_STL_INTERNAL_PAIR_H \
     			-DANDROID \
     			-DOS_ANDROID
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -ldl \
				-L$(STLPORT_BASE)/build/lib/obj/arm-linux-gcc/so -lstlport 
LOCAL_SRC_FILES := \
        detector/AlignmentPattern.cpp \
		detector/AlignmentPatternFinder.cpp \
		detector/Detector.cpp \
		detector/FinderPattern.cpp \
		detector/FinderPatternFinder.cpp \
		detector/FinderPatternInfo.cpp \
		common/Array.cpp \
		common/BitArray.cpp \
		common/BitMatrix.cpp \
		common/BlackPointEstimator.cpp \
		common/Counted.cpp \
		common/DetectorResult.cpp \
		common/GridSampler.cpp \
		common/PerspectiveTransform.cpp \
		common/PointMatrix.cpp \
		BlackPointEstimationMethod.cpp \
		ErrorCorrectionLevel.cpp \
		GrayBytesMonochromeBitmapSource.cpp \
		MonochromeBitmapSource.cpp \
		TransformingMonochromeBitmapSource.cpp \
		ResultPoint.cpp \
		Version.cpp \
		FormatInformation.cpp
		
#LOCAL_STATIC_LIBRARIES := libstlport.5.1
		
include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)
