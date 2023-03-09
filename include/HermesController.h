// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef HERMESCONTROLLER_H_
#define HERMESCONTROLLER_H_


#include "hw/BluetoothInterface.h"
#include "hw/BatteryInterface.h"
#include "hw/ChargerInterface.h"
#include "hw/MotorInterface.h"
#include "hw/ImuInterface.h"
#include "hw/UsbController.h"
#include "hw/LoadSensor.h"
#include "cmds/GenericBluetoothHandler.h"
#include "cmds/BluetoothResponse.h"
#include "cmds/BluetoothCommand.h"
#include "cmds/GetFault.h"
#include "cmds/GetInfo.h"
#include "cmds/GetBattery.h"
#include "cmds/GetId.h"
#include "cmds/GetChargeStatus.h"
#include "cmds/SetMode.h"
#include "cmds/SetMaxSpeed.h"
#include "cmds/SetMaxAccel.h"
#include "cmds/SetThrottle.h"
#include "cmds/SetMotorEnable.h"
#include "core/CommonDefs.h"
#include "core/MessageHandler.h"
#include "messages/ChargeRdyMsg.h"
#include "messages/BluetoothStatusMsg.h"


namespace hermes {

class HermesController : core::BaseApp {
public:
	enum State_t : uint8_t {
		INIT = 0,
		FAULT = 1,
		SLEEP = 2,
		MOTOR_DISABLED = 3,
		MOTOR_OFF = 4,
		READY = 5,
		CHARGING = 6
	} mState = INIT;

	HermesController(void);

	bool init(void) override;
	void loop(void) override;
	void onCriticalFault(const core::CriticalFault& criticalFault) override;
	void changeState(State_t state);

private:
	Identifier_t mIdentifier = UNKNOWN_ID;
	Control_t mControl = OFF;
	int mTestState = 0;

	bt::BluetoothInterface mBluetoothInterface;
	hw::BatteryInterface mBatteryInterface;
	hw::ChargerInterface mChargerInterface;
	hw::MotorInterface mMotorController;
	hw::ImuInterface mImuInterface;
	hw::UsbController mUsbController;
	hw::LoadSensor mLoadSensor;

	core::GenericObserver<HermesController> mButtonPressPinWatcher;
	core::GenericTimer<HermesController> mCheckStateTimer;
	core::MsgHandler<HermesController, messages::ChargeRdyMsg> mChargeRdyMsgHandler;
	core::MsgHandler<HermesController, messages::BluetoothStatusMsg> mBluetoothStatusMsg;
	bt::GenericBluetoothHandler<HermesController, bt::GetInfoCmd> mGetInfoCmdHandler;
	bt::GenericBluetoothHandler<HermesController, bt::GetFaultCmd> mGetFaultCmdHandler;
	bt::GenericBluetoothHandler<HermesController, bt::GetBatteryCmd> mGetBatteryCmdHandler;
	bt::GenericBluetoothHandler<HermesController, bt::GetIdCmd> mGetIdCmdHandler;
	bt::GenericBluetoothHandler<HermesController, bt::SetThrottleCmd> mSetThrottleCmdHandler;
	bt::GenericBluetoothHandler<HermesController, bt::SetMaxAccelCmd> mSetMaxAccelCmdHandler;
	bt::GenericBluetoothHandler<HermesController, bt::SetMaxSpeedCmd> mSetMaxSpeedCmdHandler;
	bt::GenericBluetoothHandler<HermesController, bt::SetModeCmd> mSetModeCmdHandler;
	bt::GenericBluetoothHandler<HermesController, bt::SetMotorEnableCmd> mSetMotorEnableCmdHandler;

	void enterSleep(void);
	void pullbackFromSleep(void);
	void configureGpios(void);
	bt::BluetoothResponse* handleGetInfoCmd(const bt::GetInfoCmd* cmd);
	bt::BluetoothResponse* handleGetFaultCmd(const bt::GetFaultCmd* cmd);
	bt::BluetoothResponse* handleGetBatteryCmd(const bt::GetBatteryCmd* cmd);
	bt::BluetoothResponse* handleGetIdCmd(const bt::GetIdCmd* cmd);
	bt::BluetoothResponse* handleSetThrottleCmd(const bt::SetThrottleCmd* cmd);
	bt::BluetoothResponse* handleSetMaxAccelCmd(const bt::SetMaxAccelCmd* cmd);
	bt::BluetoothResponse* handleSetMaxSpeedCmd(const bt::SetMaxSpeedCmd* cmd);
	bt::BluetoothResponse* handleSetModeCmd(const bt::SetModeCmd* cmd);
	bt::BluetoothResponse* handleSetMotorEnCmd(const bt::SetMotorEnableCmd* cmd);
	void handleChargeRdyMsg(const messages::ChargeRdyMsg* msg);
	void handleBluetoothStatusMsg(const messages::BluetoothStatusMsg* msg);
	void onButtonPress(Pin_t pin, int16_t state);
	void onTimerExpire(void);
};

}

#endif // HERMESCONTROLLER_H_