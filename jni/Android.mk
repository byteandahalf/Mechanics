LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := mechanics 
LOCAL_SRC_FILES_RAW := $(shell find $(LOCAL_PATH) -name '*.cpp') $(shell find $(LOCAL_PATH) -name '*.c')
LOCAL_SRC_FILES := $(LOCAL_SRC_FILES_RAW:$(LOCAL_PATH)/%=%)

ifeq ($(TARGET_ARCH),x86)
	LOCAL_LDLIBS   := -L$(LOCAL_PATH)/../dependencies/x86/ -ldl -lmcpelauncher_mod -lminecraftpe
    LOCAL_CFLAGS   := -DABI_X86 # $(COMMON_FLAGS_LIST)
else
	LOCAL_LDLIBS   := -L$(LOCAL_PATH)/../dependencies/arm/ -llog -ldl -lminecraftpe -lmcpelauncher_tinysubstrate
    LOCAL_CFLAGS   := -DABI_ARM # $(COMMON_FLAGS_LIST)
endif

include $(BUILD_SHARED_LIBRARY)
