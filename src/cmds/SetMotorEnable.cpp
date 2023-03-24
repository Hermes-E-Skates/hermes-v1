// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/cmds/SetMotorEnable.h"
#include "../../include/core/DevLog.h"

namespace hermes {
namespace bt {

SetMotorEnableCmd::SetMotorEnableCmd(void)
	: BluetoothCommand(SET_MOTOR_EN)
{
	return;
}

bool SetMotorEnableCmd::isMotorEnabled(void) const
{
	return mMotorEnable;
}

bool SetMotorEnableCmd::isMotorOn(void) const
{
	return mMotorOn;
}

bool SetMotorEnableCmd::isPwmForce(void) const
{
	return mPwmForce;
}

bool SetMotorEnableCmd::decode(const uint8_t* const bytes, uint8_t len)
{
	// 00 = MOTOR DISABLE, MOTOR OFF
	// 01 = MOTOR ENABLE, MOTOR OFF
	// 11 = MOTOR ENABLE, MOTOR ON
	// 10 = INVALID STATE

	if (len == 1) {
		mMotorEnable = (*bytes)&0b00000001;
		mMotorOn     = (*bytes)&0b00000010;
		mPwmForce    = (*bytes)&0b00000100;

		return true;
	} else {
		return false;
	}
}

SetMotorEnableResp::SetMotorEnableResp(bool ack)
	: BluetoothResponse(SET_MOTOR_EN, ack)
{
	return;
}

void SetMotorEnableResp::encodeData(std::vector<uint8_t>& bytes)
{
	return;
}

}
}