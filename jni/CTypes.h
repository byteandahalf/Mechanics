#pragma once

#include <android/log.h>

#define LOG_TAG "Mechanics"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long s64;


//MCPE

typedef int StorageVersion;
typedef int DimensionId;
typedef int ParticleType;
typedef int Difficulty;
enum Brightness {};


#define PLAYER_INVENTORY_OFFSET 3456