// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_


#include <vector>
#include "CommonDefs.h"
#include "Arduino.h"
#include "BaseApp.h"


namespace eskates {
namespace core {

class EventLoop {
public:
	EventLoop(void);
	EventLoop(EventLoop&&) = delete;
	EventLoop& operator=(const EventLoop&) = delete;
	EventLoop& operator=(EventLoop&&) = delete;
	static EventLoop* getInstance(void);
	void init(void);
	bool isInitDone(void) const;
	void processEvents(void);
	void registerApp(BaseApp* app);
	void unregisterApp(BaseApp* app);

private:
	bool mIsInitDone;
	static EventLoop* mInstance;
	std::vector<BaseApp*> mRegisteredApps;
};
}
}
#endif // EVENT_LOOP_H_