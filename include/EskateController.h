#ifndef ESKATECONTROLLER_H_
#define ESKATECONTROLLER_H_


#include "hw/BluetoothInterface.h"
#include "hw/BatteryInterface.h"
#include "hw/ChargerInterface.h"
#include "cmds/GenericBluetoothHandler.h"
#include "cmds/BluetoothResponse.h"
#include "cmds/BluetoothCommand.h"
#include "cmds/GetFault.h"
#include "cmds/GetInfo.h"


namespace eskates {

class EskateController : core::BaseApp {
public:
	EskateController(void);

	bool init(void) override = 0;
	void loop(void) override = 0;
	void onCriticalFault(const core::CriticalFault& criticalFault) override = 0;

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

	core::GenericObserver<EskateController> mOnButtonPress;
	bt::GenericBluetoothHandler<EskateController, bt::GetInfoCmd> mGetInfoCmdHandler;
	bt::GenericBluetoothHandler<EskateController, bt::GetFaultCmd> mGetFaultCmdHandler;

	bt::FaultData_t mFaultStatus;
};

}

#endif // ESKATECONTROLLER_H_