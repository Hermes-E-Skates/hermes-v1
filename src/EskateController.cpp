#include "../include/EskateController.h"


namespace eskates {

EskateController::EskateController(void)
	: mGetInfoCmdHandler(this, &EskateController::handleGetInfoCmd, bt::GET_INFO)
	, mGetFaultCmdHandler(this, &EskateController::handleGetFaultCmd, bt::GET_FAULT)
	, mOnButtonPress(this, &EskateController::onButtonPress, B1_PIN)
{
	return;
}

bool EskateController::init(void)
{
	bool status = false;
	return status;
}

void EskateController::loop(void)
{
	return;
}

void EskateController::onCriticalFault(const core::CriticalFault& criticalFault)
{
	return;
}

bt::BluetoothResponse* EskateController::handleGetInfoCmd(const bt::GetInfoCmd* cmd)
{

}

bt::BluetoothResponse* EskateController::handleGetFaultCmd(const bt::GetFaultCmd* cmd)
{

}

void EskateController::onButtonPress(Pin_t pin, int16_t state)
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