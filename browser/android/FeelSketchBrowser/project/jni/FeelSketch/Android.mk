LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

STLPORT_BASE := $(LOCAL_PATH)/../stlport

LOCAL_MODULE    := FeelSketch
LOCAL_SRC_FILES := FeelSketchJNI.cpp \
	util.cpp \
	pmcode/Decoder.cpp \
	pmcode/BaseColor.cpp \
	pmcode/OutputColor.cpp \
	pmcode/ColorDetector.cpp \
	pmcode/ColorMatrix.cpp \
	pmcode/LevelAdjustment.cpp \
	feelsketch/Drawer.cpp \
	feelsketch/Parser.cpp \
	feelsketch/RectCalculator.cpp

LOCAL_CFLAGS += -I$(STLPORT_BASE)/stlport \
				-I$(LOCAL_PATH)/pmcode -I$(LOCAL_PATH)/feelsketch \
				-I$(LOCAL_PATH)/../opencv/cv/include -I$(LOCAL_PATH)/../opencv/cxcore/include \
				-I$(LOCAL_PATH)/../opencv/otherlibs/highgui -I$(LOCAL_PATH)/../PMCodeDecoder/src/include \
				-I$(LOCAL_PATH)/../zxing -I$(LOCAL_PATH)/../zxing/common -I$(LOCAL_PATH)/../zxing/detector \
				-D__NEW__ \
   				-D__SGI_STL_INTERNAL_PAIR_H \
   				-DANDROID \
   				-DOS_ANDROID
   				
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -ldl -llog \
				-L$(STLPORT_BASE)/build/lib/obj/arm-linux-gcc/so -lstlport \
				-L$(LOCAL_PATH)/../../../../../out/apps/FeelSketchReader/ -lcxcore -lcv -lcvhighgui -lPMCodeDecoder -lZxingDetector


LOCAL_STATIC_LIBRARIES := cxcore cv cvhighgui PMCodeDecoder ZxingDetector

include $(BUILD_SHARED_LIBRARY)
