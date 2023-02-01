// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/cmds/Heartbeat.h"


namespace hermes {
namespace bt {

HeartbeatCmd::HeartbeatCmd(uint8_t* const bytes, uint8_t len)
	: BluetoothCommand(HEARTBEAT, bytes, len)
{
	return;
}

bool HeartbeatCmd::decodeData(uint8_t* const bytes, uint8_t len)
{
	return true;
}

HeartbeatResp::HeartbeatResp(bool ack)
	: BluetoothResponse(HEARTBEAT, ack)
{
	return;
}

void HeartbeatResp::encodeData(std::vector<uint8_t>& bytes)
{
	return;
}

}
}
