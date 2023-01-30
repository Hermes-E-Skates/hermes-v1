// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/hw/BatteryInterface.h"
#include "../../include/hw/I2cInterface.h"


namespace eskates {
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
	, mBatteryLife(0)
	, mStatusPollTimer(this, &BatteryInterface::onTimerExpire)
	, mAlertPinWatcher(this, &BatteryInterface::onAlertPinChange, BAT_ALERT_PIN)
	, mGetInfoCmdHandler(this, &BatteryInterface::onGetInfoCmd, bt::GET_INFO)
{
	return;
}

bool BatteryInterface::init()
{
	bool status = registerObserver(&mAlertPinWatcher);
	status &= registerTimer(&mStatusPollTimer);
	status &= configureBq76920();
	
	mAlertPinWatcher.enable();
	mStatusPollTimer.start(1000, PERIODIC);
	return status;
}

void BatteryInterface::loop()
{
}

void BatteryInterface::onCriticalFault(const core::CriticalFault& criticalFault)
{
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
	mRegisters.CellBal1.CellBal1Bit.CB1 = true;
	mRegisters.CellBal1.CellBal1Bit.CB2 = true;
	mRegisters.CellBal1.CellBal1Bit.CB3 = false;
	mRegisters.CellBal1.CellBal1Bit.CB4 = false;
	mRegisters.CellBal1.CellBal1Bit.CB5 = true;

	mRegisters.SysCtrl1.SysCtrl1Bit.ADC_EN = true;
	mRegisters.SysCtrl1.SysCtrl1Bit.TEMP_SEL = false;
	mRegisters.SysCtrl1.SysCtrl1Bit.SHUT_A = false;
	mRegisters.SysCtrl1.SysCtrl1Bit.SHUT_B = false;

	mRegisters.SysCtrl2.SysCtrl2Bit.DELAY_DIS = false;
	mRegisters.SysCtrl2.SysCtrl2Bit.CC_EN = true;
	mRegisters.SysCtrl2.SysCtrl2Bit.CC_ONESHOT = false;
	mRegisters.SysCtrl2.SysCtrl2Bit.DSG_ON = false;
	mRegisters.SysCtrl2.SysCtrl2Bit.CHG_ON = false;

	bool status = true;
	status &= i2cWriteWithCrc(CELLBAL1, mRegisters.CellBal1.CellBal1Byte);
	status &= i2cWriteWithCrc(SYS_CTRL1, mRegisters.SysCtrl1.SysCtrl1Byte);
	status &= i2cWriteWithCrc(SYS_CTRL2, mRegisters.SysCtrl2.SysCtrl2Byte);
	return status;
}

bt::BluetoothResponse* BatteryInterface::onGetInfoCmd(const bt::GetInfoCmd* cmd)
{
	bt::GetInfoResp* resp = nullptr;

	if (cmd->valid()) {
		resp = new bt::GetInfoResp(true);
		bt::BatteryInfo_t battery;
		bt::ConfigInfo_t config;
		bt::SysStatus_t status;

		battery.bit.BATTERY = 100;
		config.bit.control = 0x01;
		config.bit.maxAccel = 0x03;
		config.bit.maxSpeed = 0x02;

		resp->setBatteryInfo(battery);
		resp->setConfigInfo(config);
		resp->setSysStatus(status);

	} else {
		resp = new bt::GetInfoResp(false);
	}

	return resp;
}

void BatteryInterface::onTimerExpire(uint32_t userData)
{
	uint8_t buffer[2];
	I2cInterface::getInstance()->i2cRead(BQ76920_I2C_ADDR, SYS_STAT, buffer, 2);
	mRegisters.SysStatus.StatusByte = buffer[0];

	bool latched = mRegisters.SysStatus.StatusByte;
	if (latched) {
		if (mRegisters.SysStatus.StatusBit.UV) {
			core::CriticalFault cf(core::BATTERY_UNDERVOLT, core::BATTERY, micros(), "battery undervoltage lockout");
			generateCriticalFault(cf);
		}
		if (mRegisters.SysStatus.StatusBit.OV) {
			core::CriticalFault cf(core::BATTERY_OVERVOLTAGE, core::BATTERY, micros(), "battery overvoltage error");
			generateCriticalFault(cf);
		}
		if (mRegisters.SysStatus.StatusBit.SCD) {
			core::CriticalFault cf(core::BATTERY_SHORTCIRCUIT, core::BATTERY, micros(), "battery shortcircuit in discharge path");
			generateCriticalFault(cf);
		}
		if (mRegisters.SysStatus.StatusBit.DEVICE_XREADY) {
			core::CriticalFault cf(core::BATTERY_CHIP_ERR, core::BATTERY, micros(), "unknown error in bq76920");
			generateCriticalFault(cf);
		}
		i2cWriteWithCrc(SYS_STAT, 0xFF); // Clear faults by setting all bits to 1.
		mRegisters.SysStatus.StatusByte = 0x00;
	}
	return;
}

void BatteryInterface::onAlertPinChange(Pin_t pin, int16_t pinStatus)
{
	if (pinStatus == LOW) {
		// Alert set back to LOW: do nothing
	} else if (pinStatus == HIGH) {
		uint8_t buffer[4];
		I2cInterface::getInstance()->i2cRead(BQ76920_I2C_ADDR, 0x32, buffer, 4);
		uint16_t adc = buffer[0] << 8 & 0xFF00 + buffer[2] & 0x00FF;
		uint32_t current = adc * 1; // Convert ADC to current reading.
		// TODO: Save current readings somewhere that can be kept permanently
	}
}

}
}