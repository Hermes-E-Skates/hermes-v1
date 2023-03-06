// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef BLUETOOTHINTERFACE_H_
#define BLUETOOTHINTERFACE_H_


#include <Arduino.h>
#include <vector>
#include "../core/BaseApp.h"
#include "../core/GenericTimer.h"
#include "../cmds/BluetoothCommand.h"
#include "../cmds/BluetoothResponse.h"
#include "../cmds/GenericBluetoothHandler.h"
#include "../cmds/GetInfo.h"
#include "../cmds/GetFault.h"
#include "../cmds/GetId.h"
#include "../cmds/GetChargeStatus.h"
#include "../cmds/GetBattery.h"
#include "../cmds/SetMode.h"
#include "../cmds/SetThrottle.h"
#include "../cmds/SetMaxSpeed.h"
#include "../cmds/SetMaxAccel.h"
#include "../cmds/SetMotorEnable.h"
#include "../cmds/Heartbeat.h"


namespace hermes {
namespace bt {

class BluetoothInterface : public core::BaseApp {
public:
	BluetoothInterface(void);
	
	virtual bool init(void) override;
	virtual void loop(void) override;
	virtual void onCriticalFault(const core::CriticalFault& criticalFault) override;
	void checkSerialLoop(void);
	bool registerSerialHandler(BaseBluetoothHandler* handler);
	bool isConnected(void) const;
	bool sendATCommand(const std::string& AT) const;

private:
	const BluetoothCommand* parseCommand(uint8_t* const buffer, uint8_t len);
	void handleNewMessage(const BluetoothCommand* cmd);
	void onHeartbeatTimerExpire(uint32_t userdata);
	void onTestTimerTimeout(uint32_t userdata);
	BluetoothResponse* handleHeartbeat(const HeartbeatCmd* cmd);

	const uint16_t HEARTBEAT_TIMEOUT = 1100;
	bool mConnected = false;
	std::vector<BaseBluetoothHandler*> mHandlers;
	GenericBluetoothHandler<BluetoothInterface, HeartbeatCmd> mHeartbeatHandler;
	core::GenericTimer<BluetoothInterface> mHeartbeatTimeoutTimer;
	core::GenericTimer<BluetoothInterface> mTestTimer;
};

}
}

#endif // BLUETOOTHINTERFACE_H_