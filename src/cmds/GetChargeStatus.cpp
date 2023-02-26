// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/cmds/GetChargeStatus.h"


namespace hermes {
namespace bt {

GetChargeStatusCmd::GetChargeStatusCmd(void)
	: BluetoothCommand(GET_INFO)
{
	return;
}

<<<<<<< HEAD
bool GetChargeStatusCmd::decode(const uint8_t* const bytes, uint8_t len)
=======
bool GetChargeStatusCmd::decode(uint8_t* const bytes, uint8_t len)
>>>>>>> e09fda06d7e08a289f39b5ee8a5606873968e394
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
	mChargeStatus = status;
}

void GetChargeStatusResp::setChargeCurrent(uint8_t current)
{
	mInputCurrent = current;
}

void GetChargeStatusResp::setInputVoltage(uint8_t voltage)
{
	mInputVoltage = voltage;
}

void GetChargeStatusResp::encodeData(std::vector<uint8_t>& bytes)
{
	bytes.push_back(mChargeStatus.byte);
	bytes.push_back(mInputCurrent);
	bytes.push_back(mInputVoltage);
	return;
}

}
}
