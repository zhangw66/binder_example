# Copyright 2006 The Android Open Source Project
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libbinder \
    libutils \
    libhardware
LOCAL_SRC_FILES:= client.cpp
LOCAL_MODULE:= testClient
include $(BUILD_EXECUTABLE)
 
 
include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libbinder \
    libutils \
    libhardware
LOCAL_SRC_FILES:= server.cpp
LOCAL_MODULE:= testServer
include $(BUILD_EXECUTABLE)
