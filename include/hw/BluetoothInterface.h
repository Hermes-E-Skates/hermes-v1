// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef BLUETOOTHINTERFACE_H_
#define BLUETOOTHINTERFACE_H_


#include <vector>
#include "../core/BaseApp.h"
#include "../core/GenericTimer.h"
#include "../cmds/BluetoothCommand.h"
#include "../cmds/BluetoothResponse.h"
#include "../cmds/GenericBluetoothHandler.h"
#include "../cmds/GetInfo.h"
#include "../cmds/GetFault.h"


namespace eskates {
namespace bt {

class BluetoothInterface : public core::BaseApp {
public:
	BluetoothInterface(void);
	
	virtual bool init(void) override;
	virtual void loop(void) override;
	virtual void onCriticalFault(const core::CriticalFault& criticalFault) override;

	void registerSerialHandler(BaseBluetoothHandler* handler);

private:
	const BluetoothCommand* parseCommand(uint8_t* const buffer, uint8_t len);

	void handleNewMessage(const BluetoothCommand* cmd);

	std::vector<BaseBluetoothHandler*> mHandlers;
};

}
}

#endif // BLUETOOTHINTERFACE_H_