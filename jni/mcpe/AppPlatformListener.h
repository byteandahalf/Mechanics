#pragma once

#include "AppPlatform.h"

class AppPlatformListener {

public:
	AppPlatformListener();

	AppPlatformListener(bool);

	virtual ~AppPlatformListener();

	void initListener(float);


	virtual void onLowMemory();


	virtual void onAppSuspended();


	virtual void onAppResumed();

	virtual void onAppFocusLost();

	virtual void onAppFocusGained();


	virtual void onAppTerminated();

	void terminate();

private:
	AppPlatform* platform;
};
