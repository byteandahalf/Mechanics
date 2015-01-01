#pragma once

#include "AppPlatformListener.h"

class AppContext;
class Token;

class App : public AppPlatformListener
{
public:
	bool _inited;
	bool _finished;
	AppContext* _context;
	std::vector<Token> commandLine;
};
