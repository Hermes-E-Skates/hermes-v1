// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/cmds/SetMaxAccel.h"
#include "../../include/core/DevLog.h"


namespace hermes {
namespace bt {

SetMaxAccelCmd::SetMaxAccelCmd(void)
	: BluetoothCommand(SET_MAX_ACCEL)
{
	return;
}

MaxAccel_t SetMaxAccelCmd::getMaxAccel(void) const
{
	return mMaxAccel;
}

bool SetMaxAccelCmd::decode(const uint8_t* const bytes, uint8_t len)
{
	if (len == 1) {
		mMaxAccel = static_cast<MaxAccel_t>(*bytes);
		return true;
	} else {
		return false;
	}
}

SetMaxAccelResp::SetMaxAccelResp(bool ack)
	: BluetoothResponse(SET_MAX_ACCEL, ack)
{
	return;
}

void SetMaxAccelResp::encodeData(std::vector<uint8_t>& bytes)
{
	return;
}

}
}