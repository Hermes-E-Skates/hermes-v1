// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/hw/BatteryInterface.h"
#include "../../include/hw/I2cInterface.h"
#include "../../include/core/DevLog.h"


namespace hermes {
namespace hw {

unsigned char CRC8(unsigned char* ptr, unsigned char len, unsigned char key)
{
	unsigned char i;
	unsigned char crc = 0;
	while (len-- != 0)
	{
		for (i = 0x80; i != 0; i /= 2)
		{
			if ((crc & 0x80) != 0)
			{
				crc *= 2;
				crc ^= key;
			}
			else
				crc *= 2;

			if ((*ptr & i) != 0)
				crc ^= key;
		}
		ptr++;
	}
	return(crc);
}

BatteryInterface::BatteryInterface()
	: BaseApp()
	, mStatusPollTimer(this, &BatteryInterface::onTimerExpire)
{
	return;
}

bool BatteryInterface::init()
{
	DLOG_INFO("INIT: BQ25713");
	
	bool status = registerTimer(&mStatusPollTimer);
	status &= configureBq76920();
	mStatusPollTimer.start(1000, PERIODIC);
	return status;
}

void BatteryInterface::loop()
{
	return;
}

void BatteryInterface::onCriticalFault(const core::CriticalFault& criticalFault)
{
	return;
}

uint8_t BatteryInterface::getPercentEstimateFromVoltage() const
{
	/* lookup table is (obviously) not declared on the stack,
	   it's stored in static memory. Voltage is in voltage x100. 
	   Note: the voltage may drop under high current conditions.
	*/
	static std::map<uint16_t, uint8_t> lipoPercentageLookupTable = {
		{982, 0},
		{1083, 5},
		{1106, 10},
		{1120, 15},
		{1118, 20},
		{1130, 30},
		{1139, 40},
		{1151, 50},
		{1162, 60},
		{1186, 70},
		{1207, 80},
		{1225, 85},
		{1233, 90},
		{1245, 95},
		{1260, 100}
	};

	uint8_t percentage = 0;
	uint16_t batteryVoltage = mCell1 + mCell2 + mCell3;
	auto low = lipoPercentageLookupTable.begin();
	auto high = lipoPercentageLookupTable.begin();
	high++;

	while (high != lipoPercentageLookupTable.end()) {
		uint16_t voltageLow = low->first;
		uint16_t voltageHigh = high->first;
		
		// if the bat voltage is literally the lookup voltage, then assign the percentage
		if (batteryVoltage == voltageHigh) percentage = high->second;
		else if (batteryVoltage == voltageLow) percentage = low->second;

		// if the bat voltage is in-between, then lerp the estimated percentage
		else if (batteryVoltage > voltageLow && batteryVoltage < voltageHigh) { 
			uint8_t percentLow = low->second;
			uint8_t percentHigh = high->second;
			percentage = lerp(voltageLow, voltageHigh, batteryVoltage, percentLow, percentHigh);
			break;

		// if not in range, move window right
		} else {
			low++;
			high++;
		}
	}

	return percentage;
}

uint8_t BatteryInterface::lerp(uint16_t lowVoltage, uint16_t highVoltage, uint16_t voltage, uint8_t lowPercent, uint8_t highPercent) const
{
	return (lowPercent * (highVoltage - voltage) + highPercent * (voltage - lowVoltage)) / (highVoltage - lowVoltage);
}

bool BatteryInterface::clearFault(void)
{

	return false;
}

bt::BatteryStatus_t BatteryInterface::getBatteryStatus(void)
{
	bt::BatteryStatus_t batteryStatus;
	batteryStatus.bit.OVERCURRENT = mSysStatusReg.StatusBit.OCD;
	batteryStatus.bit.OVERVOLTAGE = mSysStatusReg.StatusBit.OV;
	batteryStatus.bit.UNDERVOLTAGE = mSysStatusReg.StatusBit.UV;
	batteryStatus.bit.EXTERNAL_DISABLE = mSysStatusReg.StatusBit.OVRD_ALERT;
	batteryStatus.bit.IC_FAULT = mSysStatusReg.StatusBit.DEVICE_XREADY;
	batteryStatus.bit.SHORTCIRCUIT = mSysStatusReg.StatusBit.SCD;
	return batteryStatus;
}

BatteryInterface::BatteryState_t BatteryInterface::getState(void)
{
	return mState;
}

uint16_t BatteryInterface::getCell1Voltage(void)
{
	return mCell1;
}

uint16_t BatteryInterface::getCell2Voltage(void)
{
	return mCell2;
}

uint16_t BatteryInterface::getCell3Voltage(void)
{
	return mCell3;
}

uint16_t BatteryInterface::getTemp(void)
{
	return mTemp;
}

bool BatteryInterface::i2cBlockWriteWithCrc(uint8_t registerAddr, uint8_t* data, uint8_t len)
{
	uint8_t* buffer = new uint8_t[len * 2];
	buffer[0] = data[0];
	buffer[1] = CRC8(registerAddr, data[0]);

	for (uint8_t i = 1; i < len; i++) {
		buffer[i * 2] = data[i];
		buffer[i * 2 + 1] = CRC8(data[i]);
	}

	bool success = I2cInterface::getInstance()->i2cWrite(BQ76920_I2C_ADDR, registerAddr, buffer, len * 2);
	delete buffer;
	return success;
}

bool BatteryInterface::i2cWriteWithCrc(uint8_t registerAddr, uint8_t data)
{
	uint8_t buffer[2];
	buffer[0] = data;
	buffer[1] = CRC8(registerAddr, data);
	return I2cInterface::getInstance()->i2cWrite(BQ76920_I2C_ADDR, registerAddr, buffer, 2);
}

uint8_t BatteryInterface::i2cReadWithCrc(uint8_t registerAddr)
{
	uint8_t buffer[2];
	bool valid = I2cInterface::getInstance()->i2cRead(BQ76920_I2C_ADDR, registerAddr, buffer, 2);
	if (valid) {
		return buffer[0];
	} else {
		DLOG_WARNING("I2C read failed");
		return 0;
	}
}

uint8_t BatteryInterface::CRC8(uint8_t reg, uint8_t data)
{
	const uint8_t addr = (BQ76920_I2C_ADDR << 1) | 0b00000001;
	uint8_t buffer[] = { addr | 0b00000001, reg, data };
	return hw::CRC8(buffer, 3, CRC_KEY);
}

uint8_t BatteryInterface::CRC8(uint8_t dataByte)
{
	return hw::CRC8(&dataByte, 1, CRC_KEY);
}

bool BatteryInterface::configureBq76920(void)
{
	mCellConfigReg.CellBal1Bit.CB1 = true;
	mCellConfigReg.CellBal1Bit.CB2 = true;
	mCellConfigReg.CellBal1Bit.CB3 = false;
	mCellConfigReg.CellBal1Bit.CB4 = false;
	mCellConfigReg.CellBal1Bit.CB5 = true;

	mSysCtrl1Reg.SysCtrl1Bit.ADC_EN = true;
	mSysCtrl1Reg.SysCtrl1Bit.TEMP_SEL = false;
	mSysCtrl1Reg.SysCtrl1Bit.SHUT_A = false;
	mSysCtrl1Reg.SysCtrl1Bit.SHUT_B = false;

	mSysCtrl2Reg.SysCtrl2Bit.DELAY_DIS = false;
	mSysCtrl2Reg.SysCtrl2Bit.CC_EN = true;
	mSysCtrl2Reg.SysCtrl2Bit.CC_ONESHOT = false;
	mSysCtrl2Reg.SysCtrl2Bit.DSG_ON = true;
	mSysCtrl2Reg.SysCtrl2Bit.CHG_ON = true;

	bool status = true;
	status &= i2cWriteWithCrc(CELLBAL1,  mCellConfigReg.CellBal1Byte);
	DLOG_INFO("BQ25713: cellBal1s=%s", status ? "SUCCESS" : "FAIL");

	status &= i2cWriteWithCrc(SYS_CTRL1, mSysCtrl1Reg.SysCtrl1Byte);
	DLOG_INFO("BQ25713: sysctrl1=%s", status ? "SUCCESS" : "FAIL");
	
	status &= i2cWriteWithCrc(SYS_CTRL2, mSysCtrl2Reg.SysCtrl2Byte);
	DLOG_INFO("BQ25713: sysctrl2=%s", status ? "SUCCESS" : "FAIL");

	mSysCtrl2Reg.SysCtrl2Byte = i2cReadWithCrc(SYS_CTRL2);
	DLOG_INFO("BQ25713 Write Confirm: DSG_ON=%s CHG_ON=%s", mSysCtrl2Reg.SysCtrl2Bit.DSG_ON ? "ON" : "OFF", mSysCtrl2Reg.SysCtrl2Bit.CHG_ON ? "ON" : "OFF");
	return status;
}

void BatteryInterface::onTimerExpire(uint32_t userData)
{
	uint8_t buffer[2] = {0};
	I2cInterface::getInstance()->i2cRead(BQ76920_I2C_ADDR, SYS_STAT, buffer, 2);
	mSysStatusReg.StatusByte = buffer[0];

	bool latched = mSysStatusReg.StatusBit.OCD
		| mSysStatusReg.StatusBit.DEVICE_XREADY
		| mSysStatusReg.StatusBit.OCD
		| mSysStatusReg.StatusBit.SCD
		| mSysStatusReg.StatusBit.UV;
	if (latched) {
		if (mSysStatusReg.StatusBit.UV) {
			core::CriticalFault cf(core::BATTERY_UNDERVOLT, core::BATTERY, micros(), "battery undervoltage lockout");
			generateCriticalFault(cf);
		}
		if (mSysStatusReg.StatusBit.OV) {
			core::CriticalFault cf(core::BATTERY_OVERVOLTAGE, core::BATTERY, micros(), "battery overvoltage error");
			generateCriticalFault(cf);
		}
		if (mSysStatusReg.StatusBit.SCD) {
			core::CriticalFault cf(core::BATTERY_SHORTCIRCUIT, core::BATTERY, micros(), "battery shortcircuit in discharge path");
			generateCriticalFault(cf);
		}
		if (mSysStatusReg.StatusBit.DEVICE_XREADY) {
			core::CriticalFault cf(core::BATTERY_CHIP_ERR, core::BATTERY, micros(), "unknown error in bq76920");
			generateCriticalFault(cf);
		}
		i2cWriteWithCrc(SYS_STAT, 0xFF); // Clear faults by setting all bits to 1.
		mSysStatusReg.StatusByte = 0x00;
	}

	uint8_t low = i2cReadWithCrc(VC1_LO_BYTE);
	uint8_t high = i2cReadWithCrc(VC1_HI_BYTE);
	uint32_t combined = static_cast<uint32_t>(low) | (static_cast<uint32_t>(high) << 8);
	mCell1 = static_cast<uint16_t>(combined * 382 / 10000);

	low = i2cReadWithCrc(VC2_LO_BYTE);
	high = i2cReadWithCrc(VC2_HI_BYTE);
	combined = static_cast<uint32_t>(low) | (static_cast<uint32_t>(high) << 8);
	mCell2 = static_cast<uint16_t>(combined * 382 / 10000);

	low = i2cReadWithCrc(VC5_LO_BYTE);
	high = i2cReadWithCrc(VC5_HI_BYTE);
	combined = static_cast<uint32_t>(low) | (static_cast<uint32_t>(high) << 8);
	mCell3 = static_cast<uint16_t>(combined * 382 / 10000);

	DLOG_DEBUG("Cell1=%dV Cell2=%dV Cell3=%dV", mCell1, mCell2, mCell3);

	uint8_t batPercentEstimate = getPercentEstimateFromVoltage();
	if (batPercentEstimate == 0) mState = DEAD;
	else if (batPercentEstimate <= 5) mState = CRITICAL;
	else if (batPercentEstimate <= 15) mState = WARNING;
	else mState = READY;

	low = i2cReadWithCrc(TS1_LO);
	high = i2cReadWithCrc(TS1_HI);
	combined = static_cast<uint32_t>(low) | (static_cast<uint32_t>(high) << 8);
	mTemp = 25 - ((combined * 382 - 1.2 * 384) / 0.0042);
	return;
}

}
}
