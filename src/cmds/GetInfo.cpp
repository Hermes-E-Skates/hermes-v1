// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/cmds/GetInfo.h"


namespace hermes {
namespace bt {

GetInfoCmd::GetInfoCmd()
	: BluetoothCommand(GET_INFO)
{
	return;
}

bool GetInfoCmd::decode(const uint8_t* const bytes, uint8_t len)
{
	// No data
	return true;
}

GetInfoResp::GetInfoResp(bool ack)
	: BluetoothResponse(GET_INFO, ack)
{
	return;
}

void GetInfoResp::setSysStatus(SysStatus_t status)
{
	mSysStatus = status;
}

void GetInfoResp::setSpeedInfo(uint8_t speed)
{
	mSpeed = speed;
}

void GetInfoResp::setConfigInfo(ConfigInfo_t config)
{
	mConfigInfo = config;
}

void GetInfoResp::encodeData(std::vector<uint8_t>& bytes)
{
	bytes.push_back(mSysStatus.byte);
	bytes.push_back(mConfigInfo.byte);
	bytes.push_back(mSpeed);
	return;
}

}
}
