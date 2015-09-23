LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

MECHANICS_TILE_ENTITY := Mechanics/tile/entity/
MECHANICS_TILE = Mechanics/tile/

LOCAL_MODULE    := mechanics

LOCAL_SRC_FILES_RAW := $(shell dir *.cpp /b/s)
LOCAL_SRC_FILES := $(LOCAL_SRC_FILES_RAW:$(LOCAL_PATH)/%=%)
#main.cpp $(MECHANICS_TILE_ENTITY)GrinderEntity.cpp $(MECHANICS_TILE)Grinder.cpp 

LOCAL_LDLIBS    := -L$(LOCAL_PATH) -llog -ldl -lminecraftpe -lmcpelauncher_tinysubstrate

# ignore undefined symbols.
# workaround for method not found errors.
TARGET_NO_UNDEFINED_LDFLAGS :=

include $(BUILD_SHARED_LIBRARY)
