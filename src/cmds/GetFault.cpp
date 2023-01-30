#include "../../include/cmds/GetFault.h"


namespace eskates {
namespace bt {

GetFaultCmd::GetFaultCmd(uint8_t* const bytes, uint8_t len)
	: BluetoothCommand(GET_INFO, bytes, len)
{
	return;
}

bool GetFaultCmd::decodeData(uint8_t* const bytes, uint8_t len)
{
	// No data
	return true;
}

GetFaultResp::GetFaultResp(bool ack, uint8_t speed, uint8_t)
	: BluetoothResponse(GET_INFO, ack)
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
