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

constexpr uint8_t SYS_STAT = 0;
constexpr uint8_t CELLBAL1 = 1;
constexpr uint8_t CELLBAL2 = 2;
constexpr uint8_t CELLBAL3 = 3;
constexpr uint8_t SYS_CTRL1 = 4;
constexpr uint8_t SYS_CTRL2 = 5;
constexpr uint8_t PROTECT1 = 0x06;
constexpr uint8_t PROTECT2 = 0x07;
constexpr uint8_t PROTECT3 = 0x08;
constexpr uint8_t OV_TRIP = 0x09;
constexpr uint8_t UV_TRIP = 0x0A;
constexpr uint8_t VC1_HI_BYTE = 0x0C;
constexpr uint8_t VC1_LO_BYTE = 0x0D;
constexpr uint8_t VC2_HI_BYTE = 0x0E;
constexpr uint8_t VC2_LO_BYTE = 0x0F;
constexpr uint8_t VC5_HI_BYTE = 0x14;
constexpr uint8_t VC5_LO_BYTE = 0x15;
constexpr uint8_t TS1_HI = 0x2C;
constexpr uint8_t TS1_LO = 0x2D;
constexpr uint8_t CC_HI = 0x32;
constexpr uint8_t CC_LO = 0x33;

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
