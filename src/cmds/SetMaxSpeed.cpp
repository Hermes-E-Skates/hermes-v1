// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/cmds/SetMaxSpeed.h"
#include "../../include/core/DevLog.h"


namespace hermes {
namespace bt {

SetMaxSpeedCmd::SetMaxSpeedCmd(void)
	: BluetoothCommand(SET_MAX_SPEED)
{
	return;
}

MaxSpeed_t SetMaxSpeedCmd::getMaxSpeed(void) const
{
	return mMaxSpeed;
}

bool SetMaxSpeedCmd::decode(const uint8_t* const bytes, uint8_t len)
{
	if (len == 1) {
		mMaxSpeed = static_cast<MaxSpeed_t>(*bytes);
		return true;
	} else {
		return false;
	}
}

SetMaxSpeedResp::SetMaxSpeedResp(bool ack)
	: BluetoothResponse(SET_MAX_SPEED, ack)
{
	return;
}

void SetMaxSpeedResp::encodeData(std::vector<uint8_t>& bytes)
{
	return;
}

}
}