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
#include "core/CommonDefs.h"


namespace hermes {

class HermesController : core::BaseApp {
public:
	HermesController(void);

	bool init(void) override;
	void loop(void) override;
	void onCriticalFault(const core::CriticalFault& criticalFault) override;

private:
	enum State_t : uint8_t {
		INIT = 0,
		FAULT = 1,
		SLEEP = 2,
		MOTOR_DISABLED = 3,
		READY = 4,
		CHARGING = 5
	} mState = INIT;

	Identifier_t mIdentifier = UNKNOWN_ID;
	Control_t mControl = OFF;

	bt::BluetoothInterface mBluetoothInterface;
	hw::BatteryInterface mBatteryInterface;
	hw::ChargerInterface mChargerInterface;
	hw::MotorInterface mMotorController;
	hw::ImuInterface mImuInterface;
	hw::UsbController mUsbController;

	core::GenericObserver<HermesController> mButtonPressPinWatcher;
	bt::GenericBluetoothHandler<HermesController, bt::GetInfoCmd> mGetInfoCmdHandler;
	bt::GenericBluetoothHandler<HermesController, bt::GetFaultCmd> mGetFaultCmdHandler;
	bt::GenericBluetoothHandler<HermesController, bt::GetBatteryCmd> mGetBatteryCmdHandler;
	bt::GenericBluetoothHandler<HermesController, bt::GetIdCmd> mGetIdCmdHandler;
	bt::GenericBluetoothHandler<HermesController, bt::SetThrottleCmd> mSetThrottleCmdHandler;
	bt::GenericBluetoothHandler<HermesController, bt::SetMaxAccelCmd> mSetMaxAccelCmdHandler;
	bt::GenericBluetoothHandler<HermesController, bt::SetMaxSpeedCmd> mSetMaxSpeedCmdHandler;
	bt::GenericBluetoothHandler<HermesController, bt::SetModeCmd> mSetModeCmdHandler;

	void enterSleep(void);
	void pullbackFromSleep(void);
	void changeState(State_t state);
	void configureGpios(void);
	bt::BluetoothResponse* handleGetInfoCmd(const bt::GetInfoCmd* cmd);
	bt::BluetoothResponse* handleGetFaultCmd(const bt::GetFaultCmd* cmd);
	bt::BluetoothResponse* handleGetBatteryCmd(const bt::GetBatteryCmd* cmd);
	bt::BluetoothResponse* handleGetIdCmd(const bt::GetIdCmd* cmd);
	bt::BluetoothResponse* handleSetThrottleCmd(const bt::SetThrottleCmd* cmd);
	bt::BluetoothResponse* handleSetMaxAccelCmd(const bt::SetMaxAccelCmd* cmd);
	bt::BluetoothResponse* handleSetMaxSpeedCmd(const bt::SetMaxSpeedCmd* cmd);
	bt::BluetoothResponse* handleSetModeCmd(const bt::SetModeCmd* cmd);
	void onButtonPress(Pin_t pin, int16_t state);
};

}

#endif // HERMESCONTROLLER_H_