// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../include/HermesController.h"
#include "../include/Core/DevLog.h"
#include "../include/messages/SleepStateMsg.h"
#include <avr/power.h>
#include <avr/sleep.h>


namespace hermes {

HermesController::HermesController(void)
	: BaseApp()
	, mGetIdCmdHandler(this, &HermesController::handleGetIdCmd, bt::GET_ID)
	, mSetThrottleCmdHandler(this, &HermesController::handleSetThrottleCmd, bt::SET_THROTTLE)
	, mSetMaxAccelCmdHandler(this, &HermesController::handleSetMaxAccelCmd, bt::SET_MAX_ACCEL)
	, mSetMaxSpeedCmdHandler(this, &HermesController::handleSetMaxSpeedCmd, bt::SET_MAX_SPEED)
	, mSetModeCmdHandler(this, &HermesController::handleSetModeCmd, bt::SET_MODE)
	, mSetMotorEnableCmdHandler(this, &HermesController::handleSetMotorEnCmd, bt::SET_MOTOR_EN)
	, mSetPIDK1CmdHandler(this, &HermesController::handleSetPIDK1Cmd, bt::SET_PID_K1)
	, mBluetoothStatusMsg(this, &HermesController::handleBluetoothStatusMsg, BLUETOOTH_STATUS_MSG)
{
	return;
}

bool HermesController::init(void)
{
	bool status = true;
	configureGpios();
	DevLog::init();

	status &= mBluetoothInterface.registerSerialHandler(&mGetIdCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mSetThrottleCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mSetMaxAccelCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mSetMaxSpeedCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mSetModeCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mSetMotorEnableCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mSetPIDK1CmdHandler);
	registerMessageHandler(&mBluetoothStatusMsg);

	//mButtonPressPinWatcher.enable();

	mIdentifier = digitalRead(Pin_t::GPIO_1_PIN) == 0x00 ? LEFT_SKATE : RIGHT_SKATE;
	return status;
}

void HermesController::loop(void)
{
	return;
}

void HermesController::onCriticalFault(const core::CriticalFault& criticalFault)
{
	// DLOG_ERROR("CRITICAL FAULT: code=0x%X source=0x%X timestamp=%lu \'%s\'"
	//	, criticalFault.getFaultCode()
	//	, criticalFault.getSource()
	//	, criticalFault.getTimeOfOccurence()
	//	, criticalFault.getMessage().c_str());
	//mState = FAULT;
	return;
}

void HermesController::changeState(State_t state)
{
	if (state == mState) return;

	switch (state) {
	case INIT:
		// Starts in in, will never change into init.
		// DLOG_ERROR("Shouldn't be changing to init state");
		break;

	case FAULT:
		break;

	case MOTOR_DISABLED:
		mMotorController.disableMotor();
		mMotorController.motorOff();
		digitalWrite(MOT_EN_LED, LOW);
		digitalWrite(CHG_LED, LOW);
		break;

	case MOTOR_OFF:
		mMotorController.enableMotor();
		mMotorController.motorOff();
		digitalWrite(MOT_EN_LED, HIGH);
		digitalWrite(CHG_LED, LOW);
		break;

	case READY:
		mMotorController.enableMotor();
		mMotorController.motorOn();
		digitalWrite(MOT_EN_LED, HIGH);
		digitalWrite(CHG_LED, HIGH);
		break;

	case SLEEP:
		break;

	case CHARGING:
		mMotorController.disableMotor();
		mMotorController.motorOff();
		digitalWrite(MOT_EN_LED, LOW);
		//digitalWrite(CHG_LED, HIGH);
		break;

	default:
		// DLOG_ERROR("Trying to change into unknown state.");
		break;
	}

	mState = state;
	return;
}

void HermesController::configureGpios(void)
{
	pinMode(Pin_t::ESC_PWM_PIN, OUTPUT);
	pinMode(Pin_t::B1_PIN, INPUT);
	pinMode(Pin_t::BAT_ALERT_PIN, INPUT);
	pinMode(Pin_t::USBC_FLIP_PIN, INPUT);
	pinMode(Pin_t::USBC_FAULT_PIN, INPUT);
	pinMode(Pin_t::I2C_EN_PIN, OUTPUT);
	pinMode(Pin_t::GPIO_1_PIN, INPUT);
	pinMode(Pin_t::GPIO_2_PIN, OUTPUT);
	pinMode(Pin_t::CHG_LED, OUTPUT);
	pinMode(Pin_t::MOT_EN_LED, OUTPUT);
	pinMode(Pin_t::MOTOR_SENSE_H1, INPUT);
	pinMode(Pin_t::MOTOR_SENSE_H2, INPUT);
	pinMode(Pin_t::MOTOR_SENSE_H3, INPUT);
	pinMode(Pin_t::MOTOR_SENSE_TPM, INPUT);
	pinMode(Pin_t::MOT_EN, OUTPUT);
}

bt::BluetoothResponse* HermesController::handleGetIdCmd(const bt::GetIdCmd* cmd)
{
	bt::GetIdResp* resp = nullptr;
	if (cmd != nullptr && cmd->valid()) {

		resp = new bt::GetIdResp(true);
		resp->setIdentifier(mIdentifier);

	} else {
		resp = new bt::GetIdResp(false);
	}

	return resp;
}

bt::BluetoothResponse* HermesController::handleSetThrottleCmd(const bt::SetThrottleCmd* cmd)
{
	bt::SetThrottleResp* resp = nullptr;
	if (cmd != nullptr && cmd->valid()) {
		
		if (mControl == BLUETOOTH) {
			mMotorController.setThrottleInput(cmd->getValue());
			resp = new bt::SetThrottleResp(true);
		} else {
			resp = new bt::SetThrottleResp(false);
		}

	} else {
		resp = new bt::SetThrottleResp(false);
	}

	return resp;
}

bt::BluetoothResponse* HermesController::handleSetMaxAccelCmd(const bt::SetMaxAccelCmd* cmd)
{
	bt::SetMaxAccelResp* resp = nullptr;
	if (cmd != nullptr && cmd->valid()) {
		
		mMotorController.setMaxAccel(cmd->getMaxAccel());
		resp = new bt::SetMaxAccelResp(true);
	}
	else {
		resp = new bt::SetMaxAccelResp(false);
	}

	return resp;
}

bt::BluetoothResponse* HermesController::handleSetMaxSpeedCmd(const bt::SetMaxSpeedCmd* cmd)
{
	bt::SetMaxSpeedResp* resp = nullptr;
	if (cmd != nullptr && cmd->valid()) {
		
		mMotorController.setMaxSpeed(cmd->getMaxSpeed());
		resp = new bt::SetMaxSpeedResp(true);
	}
	else {
		resp = new bt::SetMaxSpeedResp(false);
	}

	return resp;
}

bt::BluetoothResponse* HermesController::handleSetModeCmd(const bt::SetModeCmd* cmd)
{
	bt::SetModeResp* resp = nullptr;
	if (cmd != nullptr && cmd->valid()) {
		mControl = cmd->getControlMode();
		resp = new bt::SetModeResp(true);
	}
	else {
		resp = new bt::SetModeResp(false);
	}

	return resp;
}

bt::BluetoothResponse* HermesController::handleSetMotorEnCmd(const bt::SetMotorEnableCmd* cmd)
{
	bt::SetMotorEnableResp* resp = nullptr;
	if (cmd != nullptr && cmd->valid()) {

		if (cmd->isMotorEnabled() && cmd->isMotorOn()) {
			changeState(READY);
		} else if (cmd->isMotorEnabled() && !cmd->isMotorOn()) {
			changeState(MOTOR_OFF);
		} else {
			changeState(MOTOR_DISABLED);
		}

		mMotorController.setPwmForce(cmd->isPwmForce());
		resp = new bt::SetMotorEnableResp(true);
	}
	else {
		resp = new bt::SetMotorEnableResp(false);
	}

	return resp;
}

bt::BluetoothResponse* HermesController::handleSetPIDK1Cmd(const bt::SetPIDK1Cmd* cmd)
{
	bt::SetPIDK1Resp* resp = nullptr;
	if (cmd != nullptr && cmd->valid()) {
		mMotorController.setPIDK1(cmd->getPIDK1());
		Serial3.write("Reached K1 handler.");
		resp = new bt::SetPIDK1Resp(true);
	}
	else {
		resp = new bt::SetPIDK1Resp(false);
	}

	return resp;
}

void HermesController::handleBluetoothStatusMsg(const messages::BluetoothStatusMsg* msg)
{
	// Bluetooth is disconnected, shut off motor.
	if (!msg->isConnected()) {
		changeState(MOTOR_DISABLED);
	}
}

}