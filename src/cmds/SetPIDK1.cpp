// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/cmds/SetPIDK1.h"
#include "../../include/core/DevLog.h"


namespace hermes {
namespace bt {

SetPIDK1Cmd::SetPIDK1Cmd(void)
	: BluetoothCommand(SET_PID_K1)
{
	return;
}

uint8_t SetPIDK1Cmd::getPIDK1(void)
{
	return mK1;
}

bool SetPIDK1Cmd::decode(const uint8_t* const bytes, uint8_t len)
{
	if (len == 1) {
		mK1 = static_cast<uint8_t>(*bytes);
		return true;
	} else {
		return false;
	}
}

SetPIDK1Resp::SetPIDK1Resp(bool ack)
	: BluetoothResponse(SET_PID_K1, ack)
{
	return;
}

void SetPIDK1Resp::encodeData(std::vector<uint8_t>& bytes)
{
	return;
}

}
}