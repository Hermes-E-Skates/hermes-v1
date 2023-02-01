// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/cmds/GetChargeStatus.h"


namespace hermes {
namespace bt {

GetChargeStatusCmd::GetChargeStatusCmd(uint8_t* const bytes, uint8_t len)
	: BluetoothCommand(GET_INFO, bytes, len)
{
	return;
}

bool GetChargeStatusCmd::decodeData(uint8_t* const bytes, uint8_t len)
{
	// No data
	return true;
}

GetChargeStatusResp::GetChargeStatusResp(bool ack)
	: BluetoothResponse(GET_CHARGER, ack)
{
	return;
}

void GetChargeStatusResp::setChargeStatus(ChargeStatus_t status)
{
	mChargeStautus = status;
}

void GetChargeStatusResp::setChargeCurrent(uint8_t current)
{
	mChargeCurrent = current;
}

void GetChargeStatusResp::setInputVoltage(uint8_t voltage)
{
	mInputVoltage = voltage;
}

void GetChargeStatusResp::encodeData(std::vector<uint8_t>& bytes)
{
	bytes.push_back(mChargeStautus.byte);
	bytes.push_back(mChargeCurrent);
	bytes.push_back(mInputVoltage);
	return;
}

}
}
