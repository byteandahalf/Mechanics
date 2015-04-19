#pragma once
#include <functional>
#include <utility>
#include <memory>
#include <map>
#include "AppPlatformListener.h"
#include <string>
#include "texture/TextureData.h"
#include "network/mco/MCODataStructs.h"
#include <vector>

	typedef std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > > StringVector;









class AppPlatform {
	bool keyboardVisible;
	static const char* preloadingHTML;

};
