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
#include "../messages/ChargeStatusMsg.h"
#include "../cmds/GetBattery.h"


namespace hermes {
namespace hw {

class BatteryInterface : public core::BaseApp {
public:
	enum BatteryState_t {
		READY,
		WARNING,
		CRITICAL,
		DEAD
	} mState;

	BatteryInterface();
	virtual bool init() override;
	virtual void loop() override;
	virtual void onCriticalFault(const core::CriticalFault& criticalFault) override;

	uint8_t lerp(uint16_t lowVoltage, uint16_t highVoltage, uint16_t voltage, uint8_t lowPercent, uint8_t highPercent) const;
	uint8_t getPercentEstimateFromVoltage() const;
	bool enableDischarge(void);
	bool enableCharge(void);
	bool clearFault(void);
	bt::BatteryStatus_t getBatteryStatus(void);
	BatteryState_t getState(void);
	uint16_t getCell1Voltage(void);
	uint16_t getCell2Voltage(void);
	uint16_t getCell3Voltage(void);
	uint16_t getCurrent(void);
	uint16_t getTemp(void);
	

private:
	const uint8_t BQ76920_I2C_ADDR = 0x08;

	bool i2cBlockWriteWithCrc(uint8_t registerAddr, uint8_t* data, uint8_t len);
	bool i2cWriteWithCrc(uint8_t registerAddr, uint8_t data);
	uint8_t i2cReadWithCrc(uint8_t registerAddr);
	uint8_t CRC8(uint8_t reg, uint8_t data);
	uint8_t CRC8(uint8_t dataByte);
	bool configureBq76920(void);

	void onTimerExpire(uint32_t userData);
	void onAlertPinChange(Pin_t pin, int16_t pinStatus);

	SysStatus_t mSysStatusReg;
	SysCtrl1_t mSysCtrl1Reg;
	SysCtrl2_t mSysCtrl2Reg;
	CellBal1_t mCellConfigReg;

	uint16_t mCell1 = 0;
	uint16_t mCell2 = 0;
	uint16_t mCell3 = 0;
	uint16_t mTemp = 0;
	uint16_t mCurrent = 0;
	core::GenericTimer<BatteryInterface> mStatusPollTimer;
	core::GenericObserver<BatteryInterface> mAlertPinWatcher;
};

}
}

#endif // BATTERYINTERFACE_H_
