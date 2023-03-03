// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/messages/SleepStateMsg.h"


namespace hermes {
	namespace messages {

		SleepStateMsg::SleepStateMsg(bool sleepRequested)
			: Message(SLEEP_STATE_MSG)
			, mSleep(sleepRequested)
		{
			return;
		}

		bool SleepStateMsg::isSleepRequested(void) const
		{
			return mSleep;
		}

	}
}