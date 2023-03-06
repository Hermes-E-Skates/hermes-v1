// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/hw/ChargerInterface.h"
#include "../../include/hw/I2cInterface.h"
#include "../../include/core/DevLog.h"


namespace hermes {
namespace hw {

ChargerInterface::ChargerInterface()
	: mWatchdogTimer(this, &ChargerInterface::onWdogTimerExpire)
	, mFaultStatusTimer(this, &ChargerInterface::onFaultStatusTimerExpire)
	, mCheckInputSourceTimer(this, &ChargerInterface::onCheckInputSourceTimer)
	, mChargeSpeed(CURRENT_5A)
{
	return;
}

bool ChargerInterface::init()
{
	bool status = registerTimer(&mWatchdogTimer);
	registerTimer(&mFaultStatusTimer);
	registerTimer(&mCheckInputSourceTimer);
	mFaultStatusTimer.start(5000, PERIODIC);
	return status;
}

void ChargerInterface::loop()
{
	return;
}

void ChargerInterface::onCriticalFault(const core::CriticalFault& criticalFault)
{
	return;
}

bool ChargerInterface::isI2cLinkEstablished(void) const
{
	uint8_t deviceId;
	bool readSuccessfull = I2cInterface::getInstance()->i2cRead(BQ25713_I2C_ADDR, (uint8_t)0x40, &deviceId, 1);
	if (readSuccessfull) {
		DLOG_INFO("BQ25713 I2C Link=UP ID=0x%X", deviceId);
	} else {
		DLOG_WARNING("BQ25713 I2C Link=ERR");
	}
	return readSuccessfull;
}

bool ChargerInterface::startCharging(void)
{
	bool status = false;

	if (isI2cLinkEstablished())
	{
		configureBq25713();
		mCheckInputSourceTimer.start(500, ONESHOT);

	} else {
		DLOG_WARNING("startCharging FAIL: I2C link FAIL")
	}
	return status;
}

void ChargerInterface::stopCharging(void)
{
	if (mWatchdogTimer.isEnabled())
	{
		mWatchdogTimer.stop();
		mCharging = false;
	}
	return;
}

void ChargerInterface::setChargeSpeed(ChargeSpeed_t chargeSpeed)
{
	mChargeSpeed = chargeSpeed;
	return;
}

uint8_t ChargerInterface::getInputCurrent(void) const
{
	uint8_t inputCurrent = 1;
	if (!I2cInterface::getInstance()->i2cRead(BQ25713_I2C_ADDR, CHARGER_INPUT_CURRENT, &inputCurrent, 1)) {
		DLOG_WARNING("I2C Error.");
	}

	return inputCurrent;
}

uint8_t ChargerInterface::getInputVoltage(void) const
{
	uint8_t inputVoltage = 0;
	if (!I2cInterface::getInstance()->i2cRead(BQ25713_I2C_ADDR, CHARGER_INPUT_VOLTAGE, &inputVoltage, 1)) {
		DLOG_WARNING("I2C Error.");
	}

	return inputVoltage;
}

bt::ChargeStatus_t ChargerInterface::getChargerStatus(void)
{
	bool valid = true;
	bt::ChargeStatus_t status;
	ChargerStatus_low chargerStatusLow;
	ChargerStatus_high chargerStatusHigh;
	valid &= I2cInterface::getInstance()->i2cRead(BQ25713_I2C_ADDR, CHARGER_STATUS_LOW, &chargerStatusLow.byte, 1);
	valid &= I2cInterface::getInstance()->i2cRead(BQ25713_I2C_ADDR, CHARGER_STATUS_HIGH, &chargerStatusHigh.byte, 1);

	if (valid) {
		status.bit.INPUT_OVERCURRENT = chargerStatusLow.bit.FAULT_ACOC;
		status.bit.INPUT_OVERVOLT = chargerStatusLow.bit.FAULT_ACOV;
		status.bit.CHARGER_BATTERY_ERR = chargerStatusLow.bit.FAULT_BATOC;
		status.bit.IN_FAST_CHARGE = chargerStatusHigh.bit.IN_FCHRG;
		status.bit.IN_PRE_CHARGE = chargerStatusHigh.bit.IN_PCHRG;
		status.bit.INPUT_SOURCE_OK = chargerStatusHigh.bit.AC_STAT;
		status.bit.SYS_SHORT = chargerStatusLow.bit.FAULT_SYS_SHORT;

		DLOG_INFO("LATCHOFF: %d SHORT: %d INPUT OC: %d INPUT OV: %d BAT ERR: %d INPUT SRC: %d In FC: %d In PC: %d In OTG: %d"
			, chargerStatusLow.bit.FAULT_LATCHOFF, chargerStatusLow.bit.FAULT_SYS_SHORT, chargerStatusLow.bit.FAULT_ACOC, chargerStatusLow.bit.FAULT_ACOV, chargerStatusLow.bit.FAULT_BATOC
			, chargerStatusHigh.bit.AC_STAT, chargerStatusHigh.bit.IN_FCHRG, chargerStatusHigh.bit.IN_PCHRG, chargerStatusHigh.bit.IN_OTG);

		if (chargerStatusLow.bit.FAULT_SYS_SHORT == 0x01) {
			core::CriticalFault cf(core::CHARGER_SHORTCIRCUIT, core::CHARGER, micros(), "Charger short circuit");
			generateCriticalFault(cf);
		}
		if (chargerStatusLow.bit.FAULT_ACOC == 0x01) {
			core::CriticalFault cf(core::CHARGER_INPUTOVERCURRENT, core::CHARGER, micros(), "Charger short circuit");
			generateCriticalFault(cf);
		}
		if (chargerStatusLow.bit.FAULT_ACOV == 0x01) {
			core::CriticalFault cf(core::CHARGER_INPUTOVERVOLTAGE, core::CHARGER, micros(), "Charger input overvoltage");
			generateCriticalFault(cf);
		}
		if (chargerStatusLow.bit.FAULT_BATOC == 0x01) {
			core::CriticalFault cf(core::CHARGER_BATTERY_ERR, core::CHARGER, micros(), "Wrong battery voltage connected, or battery removed while charging.");
			generateCriticalFault(cf);
		}

		if (chargerStatusLow.bit.FAULT_ACOC	| chargerStatusLow.bit.FAULT_ACOV
			| chargerStatusLow.bit.FAULT_BATOC | chargerStatusLow.bit.FAULT_LATCHOFF
			| chargerStatusLow.bit.FAULT_OTG_OVP | chargerStatusLow.bit.FAULT_OTG_UVP
			| chargerStatusLow.bit.FAULT_SYS_SHORT | chargerStatusLow.bit.SYSOVP_STAT) {
			
			DLOG_ERROR("Charging error, setting charge status to false and cancelling the wdog timer.");
			mCharging = false;
			mWatchdogTimer.stop();
		}
	}
	else {
		DLOG_WARNING("I2C Error.");
	}

	return status;
}

bool ChargerInterface::configureBq25713(void) const
{
	/*
	* this block configures the bq25713 charger over i2c.
	* CHRG_INHIBIT, WDTMR_ADJ, FORCE_LATCHOFF, EN_LEARN may
	* cause a no-charge condition. We write the 8 config 
	* registers to ensure the correct settings are present.
	*/

	ChargeOption0_low charge0low;
	charge0low.bit.CHRG_INHIBIT = 0;
	charge0low.bit.EN_IDPM = 0;
	charge0low.bit.EN_LDO = 1;
	charge0low.bit.IBAT_GAIN = 1;
	charge0low.bit.IADPT_GAIN = 0;
	charge0low.bit.EN_LEARN = 0;
	charge0low.bit.SYS_SHORT_DISABLE = 0;
	bool valid = I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_0_LOW, &charge0low.byte, 1);

	ChargeOption0_high charge0high;
	charge0high.bit.LOW_PTM_RIPPLE = 1;
	charge0high.bit.PWM_FREQ = 1;
	charge0high.bit.EN_OOA = 1;
	charge0high.bit.OTG_ON_CHRGOK = 0;
	charge0high.bit.IDPM_AUTO_DISABLE = 0;
	charge0high.bit.WDTMR_ADJ = 0b10;
	charge0high.bit.EN_LWPWR = 0;
	valid &= I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_0_HIGH, &charge0high.byte, 1);
	
	I2cInterface::getInstance()->i2cRead(BQ25713_I2C_ADDR, CHARGE_OPTION_0_LOW, &charge0low.byte, 1);
	I2cInterface::getInstance()->i2cRead(BQ25713_I2C_ADDR, CHARGE_OPTION_0_HIGH, &charge0high.byte, 1);
	DLOG_INFO("Charge0 low=0b%d%d%d%d%d%d%d%d", charge0low.bit.CHRG_INHIBIT, charge0low.bit.EN_IDPM, charge0low.bit.EN_LDO, charge0low.bit.IBAT_GAIN, charge0low.bit.IADPT_GAIN, charge0low.bit.EN_LEARN, charge0low.bit.SYS_SHORT_DISABLE, charge0low.bit.RSVD);
	DLOG_INFO("Charge0 high=0b%d%d%d%d%d%d%d%d", charge0high.bit.LOW_PTM_RIPPLE, charge0high.bit.PWM_FREQ, charge0high.bit.EN_OOA, charge0high.bit.OTG_ON_CHRGOK, charge0high.bit.IDPM_AUTO_DISABLE, (charge0high.bit.WDTMR_ADJ & 0b10) >> 1, charge0high.bit.WDTMR_ADJ & 0b01, charge0high.bit.EN_LWPWR);

	ChargeOption1_low charge1low;
	charge1low.bit.CMP_REF = 0;
	charge1low.bit.CMP_POL = 0;
	charge1low.bit.CMP_DEG = 0b01;
	charge1low.bit.FORCE_LATCHOFF = 0;
	charge1low.bit.EN_PTM = 0;
	charge1low.bit.EN_SHIP_DCHG = 0;
	charge1low.bit.AUTO_WAKEUP_EN = 1;
	valid = I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_1_LOW, &charge1low.byte, 1);

	ChargeOption1_high charge1high;
	charge1high.bit.EN_IBAT = 0;
	charge1high.bit.EN_PROCHOT_LPWR = 0b00;
	charge1high.bit.EN_PSYS = 0;
	charge1high.bit.RSNS_RAC = 0;
	charge1high.bit.RSNS_RSR = 0;
	charge1high.bit.PSYS_RATIO = 1;
	charge1high.bit.PTM_PINSEL = 0;
	valid &= I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_1_HIGH, &charge1high.byte, 1);
	DLOG_INFO("Charge1 write: %s", valid ? "SUCCESS" : "FAIL");

	ChargeOption2_low charge2low;
	charge2low.bit.EN_EXTILIM = 0;
	charge2low.bit.EN_ICHG_IDCHG = 0;
	charge2low.bit.Q2_OCP = 1;
	charge2low.bit.ACX_OCP = 1;
	charge2low.bit.EN_ACOC = 1;
	charge2low.bit.ACOC_VTH = 1;
	charge2low.bit.EN_BATOC = 1;
	charge2low.bit.BATOC_VTH = 1;
	valid = I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_2_LOW, &charge2low.byte, 1);

	ChargeOption2_high charge2high;
	charge2high.bit.PKPWR_TOVLD_DEG = 0b00; 
	charge2high.bit.EN_PKPWR_IDPM = 0;
	charge2high.bit.EN_PKPWR_VSYS = 0;
	charge2high.bit.PKPWR_OVLD_STAT = 0;
	charge2high.bit.PKPWR_RELAX_STAT = 0;
	charge2high.bit.PKPWR_TMAX = 0b10;
	valid &= I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_2_HIGH, &charge2high.byte, 1);
	DLOG_INFO("Charge2 write: %s", valid ? "SUCCESS" : "FAIL");


	ChargeOption3_low charge3low;
	charge3low.bit.EN_CON_VAP = 1;
	charge3low.bit.OTG_VAP_MODE = 1;
	charge3low.bit.IL_AVG = 0b10;
	charge3low.bit.OTG_RANGE_LOW = 0;
	charge3low.bit.BATFETOFF_HIZ = 0;
	charge3low.bit.PSYS_OTG_IDCHG = 0;
	valid = I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_3_LOW, &charge3low.byte, 1);

	ChargeOption3_high charge3high;
	charge3high.bit.EN_HIZ = 0;
	charge3high.bit.RESET_REG = 0;
	charge3high.bit.RESET_VINDPM = 0;
	charge3high.bit.EN_OTG = 0;
	charge3high.bit.EN_ICO_MODE = 0;
	valid &= I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_3_HIGH, &charge3high.byte, 1);
	DLOG_INFO("Charge3 write: %s", valid ? "SUCCESS" : "FAIL");

	uint8_t low;
	uint8_t high;
	valid = I2cInterface::getInstance()->i2cRead(BQ25713_I2C_ADDR, MAX_CHARGE_VOLTAGE_LOW, &low, 1);
	valid &= I2cInterface::getInstance()->i2cRead(BQ25713_I2C_ADDR, MAX_CHARGE_VOLTAGE_HIGH, &high, 1);

	uint16_t combined = low | (high << 8);
	combined = combined >> 3;
	DLOG_INFO("Target voltage is (%s): %d mV", valid ? "SUCCESS" : "FAIL", combined * 64);
}

void ChargerInterface::onFaultStatusTimerExpire(uint32_t chargeSpeed)
{
	bt::ChargeStatus_t status = getChargerStatus(); // this will check the fault bits and print them
	DLOG_DEBUG("Charger Status: [0x%X]", status.byte);

	uint8_t current = getInputCurrent();
	DLOG_DEBUG("Charge Input Current=%d", current);

	uint8_t voltage = getInputVoltage();
	DLOG_DEBUG("Charge Input Voltage=%d", voltage);
}

void ChargerInterface::onCheckInputSourceTimer(uint32_t userData)
{
	if (getChargerStatus().bit.INPUT_SOURCE_OK == 0x01) {

		// Charge speed must be set already, and valid. 
		if (mChargeSpeed > 0 && mChargeSpeed <= MAX_SPEED)
		{
			DLOG_INFO("Starting charging.");
			mWatchdogTimer.start(4000, PERIODIC, static_cast<uint32_t>(mChargeSpeed));
			mCharging = true;
		} else {
			DLOG_WARNING("startCharging FAIL: invalid charge current setting")
		}
	}
	else {
		mInputSourceTimeouts++;
		DLOG_WARNING("startCharging FAIL: input source, retry=%d/6", mInputSourceTimeouts);
		if (mInputSourceTimeouts <= 6) {
			mCheckInputSourceTimer.start(500, ONESHOT);
		}
	}
}

void ChargerInterface::onWdogTimerExpire(uint32_t chargeSpeed)
{
	ChargeCurrent_low low;
	low.byte = (chargeSpeed << 6) & 0b11000000;

	ChargeCurrent_high high;
	high.byte = (chargeSpeed >> 3) & 0b00011111;

	bool valid = I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_CURRENT_LOW, &low.byte, 1);
	valid &= I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_CURRENT_HIGH, &high.byte, 1);
	if (!valid) {
		DLOG_WARNING("Watchdog timer failed to write the current.");
	} else {
		DLOG_DEBUG("current = 0x%X", low.byte | (high.byte) << 8);

		uint8_t low1;
		valid = I2cInterface::getInstance()->i2cRead(BQ25713_I2C_ADDR, (uint8_t)0x25, &low1, 1);
		//uint8_t current = combined >> 6;
		DLOG_DEBUG("reading current setting (%s): %d mA", valid ? "GOOD" : "FAIL", low1 * 50);
	}
}

}
}