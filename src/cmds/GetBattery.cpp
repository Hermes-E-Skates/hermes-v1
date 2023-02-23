// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/cmds/GetBattery.h"


namespace hermes {
namespace bt {

GetBatteryCmd::GetBatteryCmd(void)
	: BluetoothCommand(GET_BATTERY)
{
	return;
}

bool GetBatteryCmd::decode(uint8_t* const bytes, uint8_t len)
{
	// No data
	return true;
}

GetBatteryResp::GetBatteryResp(bool ack)
	: BluetoothResponse(GET_BATTERY, ack)
{
	return;
}

void GetBatteryResp::setBatteryStatus(const BatteryStatus_t status)
{
	mBatteryStatus = status;
}

void GetBatteryResp::setCellVoltage(uint16_t cell1, uint16_t cell2, uint16_t cell3)
{
	mCell1 = cell1;
	mCell2 = cell2;
	mCell3 = cell3;
}

void GetBatteryResp::setTemp(uint16_t temp)
{
	mTemp = temp;
}

void GetBatteryResp::setCurrent(uint16_t current)
{
	mCurrent = current;
}


void GetBatteryResp::encodeData(std::vector<uint8_t>& bytes)
{
	bytes.push_back(mBatteryStatus.byte);
	bytes.push_back(static_cast<uint8_t>(mCell1 & 0x00FF));
	bytes.push_back(static_cast<uint8_t>((mCell1 & 0xFF00) >> 8));
	bytes.push_back(static_cast<uint8_t>(mCell2 & 0x00FF));
	bytes.push_back(static_cast<uint8_t>((mCell2 & 0xFF00) >> 8));
	bytes.push_back(static_cast<uint8_t>(mCell3 & 0x00FF));
	bytes.push_back(static_cast<uint8_t>((mCell3 & 0xFF00) >> 8));
	bytes.push_back(static_cast<uint8_t>(mTemp & 0x00FF));
	bytes.push_back(static_cast<uint8_t>((mTemp & 0xFF00) >> 8));
	bytes.push_back(static_cast<uint8_t>(mCurrent & 0x00FF));
	bytes.push_back(static_cast<uint8_t>((mCurrent & 0xFF00) >> 8));
	return;
}

}
}
