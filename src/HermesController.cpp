// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../include/HermesController.h"


namespace hermes {

HermesController::HermesController(void)
	: mGetInfoCmdHandler(this, &HermesController::handleGetInfoCmd, bt::GET_INFO)
	, mGetFaultCmdHandler(this, &HermesController::handleGetFaultCmd, bt::GET_FAULT)
	, mOnButtonPress(this, &HermesController::onButtonPress, B1_PIN)
{
	return;
}

bool HermesController::init(void)
{
	bool status = false;
	return status;
}

void HermesController::loop(void)
{
	return;
}

void HermesController::onCriticalFault(const core::CriticalFault& criticalFault)
{
	return;
}

bt::BluetoothResponse* HermesController::handleGetInfoCmd(const bt::GetInfoCmd* cmd)
{

}

bt::BluetoothResponse* HermesController::handleGetFaultCmd(const bt::GetFaultCmd* cmd)
{

}

void HermesController::onButtonPress(Pin_t pin, int16_t state)
{
	if (state == LOW) {
		if (mState == READY) {
			mState = MOTOR_ENABLED;
		} else if (mState == MOTOR_ENABLED) {
			mState = READY;
		}
	}
}

}