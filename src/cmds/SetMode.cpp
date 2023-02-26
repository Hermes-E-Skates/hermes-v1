// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/cmds/SetMode.h"
#include "../../include/core/DevLog.h"


namespace hermes {
namespace bt {

SetModeCmd::SetModeCmd(void)
	: BluetoothCommand(SET_MODE)
{
	return;
}

Control_t SetModeCmd::getControlMode(void) const
{
	return mControl;
}

bool SetModeCmd::decode(const uint8_t* const bytes, uint8_t len)
{
	if (len == 1) {
		mControl = static_cast<Control_t>(*bytes);
		return true;
	} else {
		return false;
	}
}

SetModeResp::SetModeResp(bool ack)
	: BluetoothResponse(SET_MODE, ack)
{
	return;
}

void SetModeResp::encodeData(std::vector<uint8_t>& bytes)
{
	return;
}

}
}
