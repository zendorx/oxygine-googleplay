LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := oxygine-googleplay_static
LOCAL_MODULE_FILENAME := oxygine-googleplay

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/../oxygine-framework/oxygine/src/ \
		src/ \
		src/android \
					

LOCAL_SRC_FILES := \
		src/googleplay.cpp \
		src/android/AndroidGooglePlay.cpp




LOCAL_EXPORT_C_INCLUDES += \
					$(LOCAL_PATH)/src \
					$(LOCAL_PATH)/src/android \
					
					
include $(BUILD_STATIC_LIBRARY)
