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
	, mGetInfoCmdHandler(this, &HermesController::handleGetInfoCmd, bt::GET_INFO)
	, mGetFaultCmdHandler(this, &HermesController::handleGetFaultCmd, bt::GET_FAULT)
	, mGetIdCmdHandler(this, &HermesController::handleGetIdCmd, bt::GET_ID)
	, mGetBatteryCmdHandler(this, &HermesController::handleGetBatteryCmd, bt::GET_BATTERY)
	, mSetThrottleCmdHandler(this, &HermesController::handleSetThrottleCmd, bt::SET_THROTTLE)
	, mSetMaxAccelCmdHandler(this, &HermesController::handleSetMaxAccelCmd, bt::SET_MAX_ACCEL)
	, mSetMaxSpeedCmdHandler(this, &HermesController::handleSetMaxSpeedCmd, bt::SET_MAX_SPEED)
	, mSetModeCmdHandler(this, &HermesController::handleSetModeCmd, bt::SET_MODE)
	, mSetMotorEnableCmdHandler(this, &HermesController::handleSetMotorEnCmd, bt::SET_MOTOR_EN)
	, mButtonPressPinWatcher(this, &HermesController::onButtonPress, B1_PIN)
	, mChargeRdyMsgHandler(this, &HermesController::handleChargeRdyMsg, CHARGE_RDY_MSG)
	, mBluetoothStatusMsg(this, &HermesController::handleBluetoothStatusMsg, BLUETOOTH_STATUS_MSG)
{
	return;
}

bool HermesController::init(void)
{
	bool status = true;
	configureGpios();
	DevLog::init();

	status &= mBluetoothInterface.registerSerialHandler(&mGetInfoCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mGetFaultCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mGetBatteryCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mGetIdCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mSetThrottleCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mSetMaxAccelCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mSetMaxSpeedCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mSetModeCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mSetMotorEnableCmdHandler);
	status &= registerObserver(&mButtonPressPinWatcher);
	registerMessageHandler(&mChargeRdyMsgHandler);
	registerMessageHandler(&mBluetoothStatusMsg);
	mButtonPressPinWatcher.enable();
	mIdentifier = digitalRead(Pin_t::GPIO_1_PIN) == 0x00 ? LEFT_SKATE : RIGHT_SKATE;
	return status;
}

void HermesController::loop(void)
{
	return;
}

void HermesController::onCriticalFault(const core::CriticalFault& criticalFault)
{
	DLOG_ERROR("CRITICAL FAULT: code=0x%X source=0x%X timestamp=%lu \'%s\'"
		, criticalFault.getFaultCode()
		, criticalFault.getSource()
		, criticalFault.getTimeOfOccurence()
		, criticalFault.getMessage().c_str());
	//mState = FAULT;
	return;
}

void HermesController::enterSleep(void)
{
	messages::SleepStateMsg msg(true);
	BaseApp::sendMessage(&msg);

	// Put core in sleep state
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	return;
}

void HermesController::pullbackFromSleep(void)
{
	// Take core out of sleep state
	// todo: 

	messages::SleepStateMsg msg(false);
	BaseApp::sendMessage(&msg);
	return;
}

void HermesController::changeState(State_t state)
{
	if (state == mState) return;

	switch (state) {
	case INIT:
		// Starts in in, will never change into init.
		DLOG_ERROR("Shouldn't be changing to init state");
		break;

	case FAULT:
		mMotorController.disableMotor();
		mMotorController.motorOff();
		digitalWrite(MOT_EN_LED, LOW);
		digitalWrite(CHG_LED, LOW);
		mChargerInterface.stopCharging();
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
		mChargerInterface.stopCharging();
		break;

	case SLEEP:
		mMotorController.disableMotor();
		digitalWrite(MOT_EN_LED, LOW);
		digitalWrite(CHG_LED, LOW);
		enterSleep();
		break;

	case CHARGING:
		mMotorController.disableMotor();
		mMotorController.motorOff();
		digitalWrite(MOT_EN_LED, LOW);
		//digitalWrite(CHG_LED, HIGH);
		break;

	default:
		DLOG_ERROR("Trying to change into unknown state.");
		break;
	}

	// detect pullback from sleep
	if (mState == SLEEP) {
		pullbackFromSleep();
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

bt::BluetoothResponse* HermesController::handleGetInfoCmd(const bt::GetInfoCmd* cmd)
{
	bt::GetInfoResp* resp = nullptr;
	if (cmd != nullptr && cmd->valid()) {
		resp = new bt::GetInfoResp(true);
		
		bt::SysStatus_t sysStatus;
		bt::BatteryStatus_t batteryStatus = mBatteryInterface.getBatteryStatus();
		bt::ChargeStatus_t chargerStatus = mChargerInterface.getChargerStatus();
		sysStatus.bit.FAULT_FLAG = batteryStatus.bit.IC_FAULT
			| batteryStatus.bit.OVERCURRENT
			| batteryStatus.bit.OVERVOLTAGE
			| batteryStatus.bit.SHORTCIRCUIT
			| batteryStatus.bit.UNDERVOLTAGE;
		sysStatus.bit.BATTERY_STATE = mBatteryInterface.getState();
		sysStatus.bit.RUN_STATE = mState;
		resp->setSysStatus(sysStatus);

		bt::ConfigInfo_t config;
		config.bit.control = mControl;
		config.bit.maxAccel = mMotorController.getMaxAccel();
		config.bit.maxSpeed = mMotorController.getMaxSpeed();
		resp->setConfigInfo(config);

	} else {
		resp = new bt::GetInfoResp(false);
	}

	return resp;
}

bt::BluetoothResponse* HermesController::handleGetFaultCmd(const bt::GetFaultCmd* cmd)
{
	bt::GetFaultResp* resp = nullptr;
	if (cmd != nullptr && cmd->valid()) {

		bt::FaultData_t fault;
		bt::BatteryStatus_t batFault = mBatteryInterface.getBatteryStatus();
		bt::ChargeStatus_t chargeFault = mChargerInterface.getChargerStatus();

		resp = new bt::GetFaultResp(true);
		fault.bit.BATTERY_DISCONNECT;
		//fault.bit.BAT_CRITICAL = 
		//fault.bit.BAT_WARNING =
		//fault.bit.CHARGE_ERR =
		//fault.bit.CRITICAL_FAULT =
		//fault.bit.IMU_ERR =
		//fault.bit.MOTOR_ERR =
		//fault.bit.SOFTWARE_ERR =
		resp->SetFaultStatus(fault);
	}
	else {
		resp = new bt::GetFaultResp(false);
	}

	return resp;
}

bt::BluetoothResponse* HermesController::handleGetBatteryCmd(const bt::GetBatteryCmd* cmd)
{
	bt::GetBatteryResp* resp = nullptr;
	if (cmd != nullptr && cmd->valid()) {

		bt::BatteryStatus_t status = mBatteryInterface.getBatteryStatus();
		uint16_t cell1 = mBatteryInterface.getCell1Voltage();
		uint16_t cell2 = mBatteryInterface.getCell2Voltage();
		uint16_t cell3 = mBatteryInterface.getCell3Voltage();
		uint16_t temp = mBatteryInterface.getTemp();

		resp = new bt::GetBatteryResp(true);
		resp->setBatteryStatus(status);
		resp->setCellVoltage(cell1, cell2, cell3);
		resp->setTemp(temp);

	} else {
		resp = new bt::GetBatteryResp(false);
	}

	return resp;
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
		resp = new bt::SetMotorEnableResp(true);
	}
	else {
		resp = new bt::SetMotorEnableResp(false);
	}

	return resp;
}

void HermesController::handleChargeRdyMsg(const messages::ChargeRdyMsg* msg)
{
	if (msg->isReadyToCharge()) {
		changeState(CHARGING);
		mChargerInterface.setChargeSpeed(ChargeSpeed_t::CURRENT_5A);
		mChargerInterface.startCharging();
	} else {
		changeState(MOTOR_DISABLED);
		mChargerInterface.stopCharging();
	}
}

void HermesController::handleBluetoothStatusMsg(const messages::BluetoothStatusMsg* msg)
{
	// Bluetooth is disconnected, shut off motor.
	if (!msg->isConnected()) {
		changeState(MOTOR_DISABLED);
	}
}

void HermesController::onButtonPress(Pin_t pin, int16_t state)
{
	DLOG_INFO("Button state=%s", state == LOW ? "DOWN" : "UP");
	//if (state == LOW) {
	//	if (mState == MOTOR_OFF) {
	//		changeState(MOTOR_DISABLED);
	//	} else if (mState == MOTOR_DISABLED) {
	//		changeState(MOTOR_OFF);
	//	}
	//}

	if (state == LOW) {
		mTestState++;
		if (mTestState >= 6) {
			mTestState = 0;
		}

		switch (mTestState) {
		case 0:
			changeState(MOTOR_DISABLED);
			break;

		case 1:
			changeState(MOTOR_OFF);
			break;

		case 2:
			changeState(READY);
			break;

		case 3:
			mMotorController.setThrottleInput(175);
			break;

		case 4:
			mMotorController.setThrottleInput(255);
			break;

		case 5:
			mMotorController.setThrottleInput(0);
			break;
		}
	}
}

}