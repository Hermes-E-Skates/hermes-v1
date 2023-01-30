#include "../../include/cmds/GetInfo.h"


namespace eskates {
namespace bt {

GetInfoCmd::GetInfoCmd(uint8_t* const bytes, uint8_t len)
	: BluetoothCommand(GET_INFO, bytes, len)
{
	return;
}

bool GetInfoCmd::decodeData(uint8_t* const bytes, uint8_t len)
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

void GetInfoResp::setSpeedInfo(SpeedInfo_t speed)
{
	mSpeedInfo = speed;
}

void GetInfoResp::setBatteryInfo(BatteryInfo_t battery)
{
	mBatteryInfo = battery;
}

void GetInfoResp::setConfigInfo(ConfigInfo_t config)
{
	mConfigInfo = config;
}

void GetInfoResp::encodeData(std::vector<uint8_t>& bytes)
{
	bytes.push_back(mSysStatus.byte);
	bytes.push_back(mConfigInfo.byte);
	bytes.push_back(mSpeedInfo.byte);
	bytes.push_back(mBatteryInfo.byte);
	return;
}

}
}
