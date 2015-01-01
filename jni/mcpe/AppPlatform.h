#pragma once

#include <map>
#include "AppPlatformListener.h"

class AppPlatformListener;
class AppPlatform
{
public:
	bool keyboardVisible;
	const char* preloadingHTML;
	std::multimap<float, AppPlatformListener*> listeners;
};
