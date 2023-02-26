// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/cmds/GetFault.h"


namespace hermes {
namespace bt {

GetFaultCmd::GetFaultCmd(void)
	: BluetoothCommand(GET_FAULT)
{
	return;
}

bool GetFaultCmd::decode(const uint8_t* const bytes, uint8_t len)
{
	// No data
	return true;
}

GetFaultResp::GetFaultResp(bool ack)
	: BluetoothResponse(GET_FAULT, ack)
{
	return;
}

void GetFaultResp::SetFaultStatus(FaultData_t fault)
{
	mFaultStatus = fault;
}

void GetFaultResp::encodeData(std::vector<uint8_t>& bytes)
{
	bytes.push_back(mFaultStatus.byte);
	return;
}

}
}
