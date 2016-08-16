LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := slava
# LOCAL_CPP_EXTENSION := .cc

LOCAL_SRC_FILES := ../../slava.cc
LOCAL_C_INCLUDES := ../../include

include $(BUILD_SHARED_LIBRARY)
