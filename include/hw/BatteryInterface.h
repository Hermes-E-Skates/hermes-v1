// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef BATTERYINTERFACE_H_
#define BATTERYINTERFACE_H_


#include "bqMaximo_Ctrl_G2553.h"
#include "../core/BaseApp.h"
#include "../core/GenericTimer.h"
#include "../core/MessageHandler.h"
#include "../messages/ChargeStatusMsg.h"
#include "../cmds/GetInfo.h"
#include "../cmds/BluetoothResponse.h"
#include "../cmds/GenericBluetoothHandler.h"


namespace hermes {
namespace hw {

class BatteryInterface : public core::BaseApp {
public:
	BatteryInterface();

	virtual bool init() override;
	virtual void loop() override;
	virtual void onCriticalFault(const core::CriticalFault& criticalFault) override;

	bool enableDischarge(void);
	bool enableCharge(void);
	bool enableCharge(void);
	bool clearFault(void);

private:
	const uint8_t BQ76920_I2C_ADDR = 0x08;

	bool i2cBlockWriteWithCrc(uint8_t registerAddr, uint8_t* data, uint8_t len);
	bool i2cWriteWithCrc(uint8_t registerAddr, uint8_t data);
	uint8_t CRC8(uint8_t reg, uint8_t data);
	uint8_t CRC8(uint8_t dataByte);
	bool configureBq76920(void);

	bt::BluetoothResponse* onGetInfoCmd(const bt::GetInfoCmd* cmd);
	void onTimerExpire(uint32_t userData);
	void onAlertPinChange(Pin_t pin, int16_t pinStatus);


	RegisterGroup mRegisters;
	uint32_t mBatteryLife;
	core::GenericTimer<BatteryInterface> mStatusPollTimer;
	core::GenericObserver<BatteryInterface> mAlertPinWatcher;
	bt::GenericBluetoothHandler<BatteryInterface, bt::GetInfoCmd> mGetInfoCmdHandler;
};

}
}

#endif // BATTERYINTERFACE_H_