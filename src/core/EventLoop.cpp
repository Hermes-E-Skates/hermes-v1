// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/core/EventLoop.h"


namespace eskates {
namespace core {

EventLoop* EventLoop::mInstance = nullptr;

EventLoop::EventLoop(void)
	: mIsInitDone(false)
{
	return;
}

EventLoop* EventLoop::getInstance(void)
{
	if (mInstance == nullptr) {
		mInstance = new EventLoop();
	}
	return mInstance;
}

void EventLoop::init(void)
{
	for (auto app : mRegisteredApps)
	{
		app->baseStart();
	}
	mIsInitDone = true;
}

bool EventLoop::isInitDone(void) const
{
	return mIsInitDone;
}

void EventLoop::processEvents(void)
{
	for (auto app : mRegisteredApps)
	{
		app->baseUpdate();
	}
}

void EventLoop::registerApp(BaseApp* app)
{
	mRegisteredApps.push_back(app);
}

void EventLoop::unregisterApp(BaseApp* app)
{
	mRegisteredApps.push_back(app);
}

}
}