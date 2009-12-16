LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := PMCodeDecoder
LOCAL_C_INCLUDES := \
        $(LOCAL_PATH)/src/include/

LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%)
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -ldl -lm -llog


LOCAL_SRC_FILES := \
        src/Global.cpp \
        src/mathematics.cpp \
        src/ReedSolomon.cpp \
        src/QRCodeDecode.cpp \
        src/QRCodeDisposition.cpp \
        src/QRCodeMask.cpp \
        src/QRCodeRestoration.cpp \
        src/QRCodeSymbol.cpp \
        src/QRCodeUnMask.cpp \
        src/PMCodeAnalysis.cpp \
        src/PMCodeToQRCodes.cpp \
        src/PMCodeReader.cpp \
        src/PMCodeDecode.cpp \
#        src/PMCode.cpp

include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_EXECUTABLE)
