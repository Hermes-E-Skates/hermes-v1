// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/cmds/SetThrottle.h"
#include "../../include/core/DevLog.h"

namespace hermes {
namespace bt {

SetThrottleCmd::SetThrottleCmd(void)
	: BluetoothCommand(SET_THROTTLE)
{
	return;
}

float SetThrottleCmd::getPercentage(void) const
{
	return mThrottle / 255.0f * 100;
}

bool SetThrottleCmd::decode(const uint8_t* const bytes, uint8_t len)
{
	if (len == 1) {
		mThrottle = *bytes;
		return true;
	} else {
		return false;
	}
}

SetThrottleResp::SetThrottleResp(bool ack)
	: BluetoothResponse(SET_THROTTLE, ack)
{
	return;
}

void SetThrottleResp::encodeData(std::vector<uint8_t>& bytes)
{
	return;
}

}
}