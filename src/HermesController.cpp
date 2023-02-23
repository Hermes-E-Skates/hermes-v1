// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../include/HermesController.h"
#include "../include/Core/DevLog.h"


namespace hermes {

HermesController::HermesController(void)
	: mGetInfoCmdHandler(this, &HermesController::handleGetInfoCmd, bt::GET_INFO)
	, mGetFaultCmdHandler(this, &HermesController::handleGetFaultCmd, bt::GET_FAULT)
	, mGetIdCmdHandler(this, &HermesController::handleGetIdCmd, bt::GET_ID)
	, mGetBatteryCmdHandler(this, &HermesController::handleGetBatteryCmd, bt::GET_BATTERY)
	, mButtonPressPinWatcher(this, &HermesController::onButtonPress, B1_PIN)
{
	return;
}

bool HermesController::init(void)
{
	bool status = true;
	configureGpios();
	status &= mBluetoothInterface.registerSerialHandler(&mGetInfoCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mGetFaultCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mGetBatteryCmdHandler);
	status &= mBluetoothInterface.registerSerialHandler(&mGetIdCmdHandler);
	status &= registerObserver(&mButtonPressPinWatcher);
	mIdentifier = digitalRead(Pin_t::GPIO_1_PIN) == 0x00 ? LEFT_SKATE : RIGHT_SKATE;
	return status;
}

void HermesController::loop(void)
{
	return;
}

void HermesController::onCriticalFault(const core::CriticalFault& criticalFault)
{
	DLOG_ERROR("CRITICAL FAULT: \ncode=0x%X\nsource=0x%X\ntimestamp=%lu\n%s"
		, criticalFault.getFaultCode()
		, criticalFault.getSource()
		, criticalFault.getTimeOfOccurence()
		, criticalFault.getMessage().c_str());
	mState = FAULT;
	return;
}

void HermesController::changeState(State_t state)
{
	switch (state) {
	case INIT:
		// Disable motor
		break;

	case FAULT:
		// Disable motor
		break;

	case MOTOR_DISABLED:
		// Disable motor
		break;

	case READY:
		// Enable motor
		mChargerInterface.stopCharging();
		break;

	case SLEEP:
		// Disable motor
		// Publish sleep message
		break;

	case CHARGING:
		mChargerInterface.startCharging();
		// Disable motor
		break;

	default:
		break;
	}

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
}

bt::BluetoothResponse* HermesController::handleGetInfoCmd(const bt::GetInfoCmd* cmd)
{
	bt::GetInfoResp* resp = nullptr;
	if (cmd != nullptr && cmd->valid()) {
		resp = new bt::GetInfoResp(true);
		//resp->setSpeedInfo(mMotorController.getSpeed());
		
		bt::SysStatus_t sysStatus;
		bt::BatteryStatus_t batteryStatus = mBatteryInterface.getBatteryStatus();
		bt::ChargeStatus_t chargerStatus = mChargerInterface.getChargerStatus();
		sysStatus.bit.FAULT_FLAG = batteryStatus.bit.IC_FAULT
			| batteryStatus.bit.OVERCURRENT
			| batteryStatus.bit.OVERVOLTAGE
			| batteryStatus.bit.SHORTCIRCUIT
			| batteryStatus.bit.UNDERVOLTAGE
			| chargerStatus.bit.FAULT;
		sysStatus.bit.BATTERY_STATE = mBatteryInterface.getState();
		sysStatus.bit.RUN_STATE = mState;
		resp->setSysStatus(sysStatus);

		bt::ConfigInfo_t config;
		config.bit.control = mControl;
		config.bit.maxAccel = mMaxAccel;
		config.bit.maxSpeed = mMaxSpeed;
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
		uint16_t current = mBatteryInterface.getCurrent();
		uint16_t temp = mBatteryInterface.getTemp();

		resp = new bt::GetBatteryResp(true);
		resp->setBatteryStatus(status);
		resp->setCellVoltage(cell1, cell2, cell3);
		resp->setCurrent(current);
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

void HermesController::onButtonPress(Pin_t pin, int16_t state)
{
	if (state == LOW) {
		if (mState == READY) {
			mState = MOTOR_DISABLED;
		} else if (mState == MOTOR_DISABLED) {
			mState = READY;
		}
	}
}

}