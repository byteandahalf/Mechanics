#pragma once

#if defined(ABI_X86)
	#define LOGI(...)
#elif defined(ABI_ARM)
	#include <android/log.h>
	#define LOG_TAG "Mechanics"
	#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#endif
