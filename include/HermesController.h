// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef HermesController_H_
#define HermesController_H_


#include "hw/BluetoothInterface.h"
#include "hw/BatteryInterface.h"
#include "hw/ChargerInterface.h"
#include "cmds/GenericBluetoothHandler.h"
#include "cmds/BluetoothResponse.h"
#include "cmds/BluetoothCommand.h"
#include "cmds/GetFault.h"
#include "cmds/GetInfo.h"


namespace hermes {

class HermesController : core::BaseApp {
public:
	HermesController(void);

	bool init(void) override;
	void loop(void) override;
	void onCriticalFault(const core::CriticalFault& criticalFault) override;

private:
	enum State_t {
		INIT          = 0b00001,
		FAULT         = 0b00010,
		CHARGING      = 0b00100,
		READY         = 0b01000,
		MOTOR_ENABLED = 0b10000
	} mState;

	bt::BluetoothResponse* handleGetInfoCmd(const bt::GetInfoCmd* cmd);
	bt::BluetoothResponse* handleGetFaultCmd(const bt::GetFaultCmd* cmd);
	void onButtonPress(Pin_t pin, int16_t state);

	bt::BluetoothInterface mBluetoothInterface;
	hw::BatteryInterface mBatteryInterface;
	hw::ChargerInterface mChargerInterface;
	hw::MotorInterface mMotorController;
	hw::ImuInterface ImuInterface;
	hw::UsbInterface mUsbInterface;

	core::GenericObserver<HermesController> mOnButtonPress;
	bt::GenericBluetoothHandler<HermesController, bt::GetInfoCmd> mGetInfoCmdHandler;
	bt::GenericBluetoothHandler<HermesController, bt::GetFaultCmd> mGetFaultCmdHandler;

	bt::FaultData_t mFaultStatus;
};

}

#endif // HermesController_H_