// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef SLEEPSTATEMSG_H_
#define SLEEPSTATEMSG_H_


#include "Message.h"


namespace hermes {
namespace messages {

class SleepStateMsg : public Message
{
public:
	SleepStateMsg(bool sleep);
	bool isSleepRequested(void) const;

private:
	bool mSleep;
};
}
}
#endif // SLEEPSTATEMSG_H_