// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/cmds/GetId.h"


namespace hermes {
namespace bt {

GetIdCmd::GetIdCmd(void)
	: BluetoothCommand(GET_ID)
{
	return;
}

bool GetIdCmd::decode(const uint8_t* const bytes, uint8_t len)
{
	// No data
	return true;
}

GetIdResp::GetIdResp(bool ack)
	: BluetoothResponse(GET_ID, ack)
	, mIdentifier(UNKNOWN_ID)
{
	return;
}

void GetIdResp::setIdentifier(Identifier_t id)
{
	mIdentifier = id;
}

void GetIdResp::encodeData(std::vector<uint8_t>& bytes)
{
	bytes.push_back(mIdentifier);
	return;
}

}
}
