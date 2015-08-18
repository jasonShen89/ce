LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := unifylib_shared

LOCAL_MODULE_FILENAME := libunifylib

LOCAL_SRC_FILES := hellocpp/main.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_WHOLE_STATIC_LIBRARIES := unify_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

