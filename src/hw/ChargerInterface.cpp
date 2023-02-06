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
	, mChargeSpeed(CURRENT_5A)
{
	return;
}

bool ChargerInterface::init()
{
	bool status = registerTimer(&mWatchdogTimer);
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

void ChargerInterface::startCharging(void)
{
	DLOG_INFO("Starting charging.");
	if (mChargeSpeed > 0 && mChargeSpeed <= MAX_SPEED)
	{
		mWatchdogTimer.start(4000, PERIODIC, static_cast<uint32_t>(mChargeSpeed));
	}
}

void ChargerInterface::stopCharging(void)
{
	if (mWatchdogTimer.isEnabled())
	{
		mWatchdogTimer.stop();
	}
}

bool ChargerInterface::setChargeSpeed(ChargeSpeed_t chargeSpeed)
{
	mChargeSpeed = chargeSpeed;
	return false;
}

uint8_t ChargerInterface::getInputCurrent(void) const
{
	uint8_t inputCurrent = 0;
	if (!I2cInterface::getInstance()->i2cRead(BQ25713_I2C_ADDR, CHARGER_INPUT_CURRENT, &inputCurrent, 1)) {
		DLOG_WARNING("I2C Error.");
	}
}

uint8_t ChargerInterface::getInputVoltage(void) const
{
	uint8_t inputVoltage = 0;
	if (!I2cInterface::getInstance()->i2cRead(BQ25713_I2C_ADDR, CHARGER_INPUT_VOLTAGE, &inputVoltage, 1)) {
		DLOG_WARNING("I2C Error.");
	}

	return inputVoltage;
}

bt::ChargeStatus_t ChargerInterface::getChargerStatus(void) const
{
	bool valid = true;
	bt::ChargeStatus_t status;
	ChargerStatus_low chargerStatusLow;
	ChargerStatus_high chargerStatusHigh;
	valid &= I2cInterface::getInstance()->i2cRead(BQ25713_I2C_ADDR, CHARGE_OPTION_3_LOW, &chargerStatusLow.byte, 1);
	valid &= I2cInterface::getInstance()->i2cRead(BQ25713_I2C_ADDR, CHARGE_OPTION_3_HIGH, &chargerStatusHigh.byte, 1);

	if (valid) {
		status.bit.FAULT = chargerStatusLow.bit.FAULT_ACOC
			| chargerStatusLow.bit.FAULT_ACOV
			| chargerStatusLow.bit.FAULT_BATOC
			| chargerStatusLow.bit.FAULT_SYS_SHORT;

		status.bit.IN_FAST_CHARGE = chargerStatusHigh.bit.IN_FCHRG;
		status.bit.IN_PRE_CHARGE = chargerStatusHigh.bit.IN_PCHRG;
		status.bit.INPUT_SOURCE_OK = chargerStatusHigh.bit.AC_STAT;
		status.bit.STATE = 0b00; // TODO FINISH THIS

		if (chargerStatusLow.bit.FAULT_SYS_SHORT == 0x01) {
			core::CriticalFault cf(core::CHARGER_SHORTCIRCUIT, core::CHARGER, micros(), "Charger short circuit");
			generateCriticalFault(cf);
		}
		if (chargerStatusLow.bit.FAULT_ACOC == 0x01) {
			core::CriticalFault cf(core::CHARGER_INPUTOVERCURRENT, core::CHARGER, micros(), "Charger short circuit");
			generateCriticalFault(cf);
		}
		if (chargerStatusLow.bit.FAULT_ACOV == 0x01) {
			core::CriticalFault cf(core::CHARGER_INPUTOVERVOLTAGE, core::CHARGER, micros(), "Charger short circuit");
			generateCriticalFault(cf);
		}
		if (chargerStatusLow.bit.FAULT_BATOC == 0x01) {
			core::CriticalFault cf(core::CHARGER_BATTERY_ERR, core::CHARGER, micros(), "Wrong battery voltage connected, or battery removed while charging.");
			generateCriticalFault(cf);
		}
	}
	else {
		DLOG_WARNING("I2C Error.");
	}

	return status;
}

bool ChargerInterface::configureBq25713(void)
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
	I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_0_LOW, &charge0low.byte, 1);

	ChargeOption0_high charge0high;
	charge0high.bit.LOW_PTM_RIPPLE = 1;
	charge0high.bit.PWM_FREQ = 1;
	charge0high.bit.EN_OOA = 1;
	charge0high.bit.OTG_ON_CHRGOK = 0;
	charge0high.bit.IDPM_AUTO_DISABLE = 0;
	charge0high.bit.WDTMR_ADJ = 0x00;
	charge0high.bit.EN_LWPWR = 0;
	I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_0_HIGH, &charge0high.byte, 1);

	ChargeOption1_low charge1low;
	charge1low.bit.CMP_REF = 0;
	charge1low.bit.CMP_POL = 0;
	charge1low.bit.CMP_DEG = 0b01;
	charge1low.bit.FORCE_LATCHOFF = 0;
	charge1low.bit.EN_PTM = 0;
	charge1low.bit.EN_SHIP_DCHG = 0;
	charge1low.bit.AUTO_WAKEUP_EN = 1;
	I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_1_LOW, &charge1low.byte, 1);

	ChargeOption1_high charge1high;
	charge1high.bit.EN_IBAT = 0;
	charge1high.bit.EN_PROCHOT_LPWR = 0b00;
	charge1high.bit.EN_PSYS = 1;
	charge1high.bit.RSNS_RAC = 0;
	charge1high.bit.RSNS_RSR = 0;
	charge1high.bit.PSYS_RATIO = 1;
	charge1high.bit.PTM_PINSEL = 0;
	I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_1_HIGH, &charge1high.byte, 1);

	ChargeOption2_low charge2low;
	charge2low.bit.EN_EXTILIM = 0;
	charge2low.bit.EN_ICHG_IDCHG = 0;
	charge2low.bit.Q2_OCP = 1;
	charge2low.bit.ACX_OCP = 1;
	charge2low.bit.EN_ACOC = 1;
	charge2low.bit.ACOC_VTH = 1;
	charge2low.bit.EN_BATOC = 1;
	charge2low.bit.BATOC_VTH = 1;
	I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_2_LOW, &charge2low.byte, 1);

	ChargeOption2_high charge2high;
	charge2high.bit.PKPWR_TOVLD_DEG = 0b00;
	charge2high.bit.EN_PKPWR_IDPM = 0;
	charge2high.bit.EN_PKPWR_VSYS = 0;
	charge2high.bit.PKPWR_OVLD_STAT = 0;
	charge2high.bit.PKPWR_RELAX_STAT = 0;
	charge2high.bit.PKPWR_TMAX = 0b10;
	I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_2_HIGH, &charge2high.byte, 1);

	ChargeOption3_low charge3low;
	charge3low.bit.EN_CON_VAP = 1;
	charge3low.bit.OTG_VAP_MODE = 1;
	charge3low.bit.IL_AVG = 0b10;
	charge3low.bit.OTG_RANGE_LOW = 0;
	charge3low.bit.BATFETOFF_HIZ = 0;
	charge3low.bit.PSYS_OTG_IDCHG = 0;
	I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_3_LOW, &charge3low.byte, 1);

	ChargeOption3_high charge3high;
	charge3high.bit.EN_HIZ = 0;
	charge3high.bit.RESET_REG = 0;
	charge3high.bit.RESET_VINDPM = 0;
	charge3high.bit.EN_OTG = 0;
	charge3high.bit.EN_ICO_MODE = 0;
	I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_3_HIGH, &charge3high.byte, 1);
}

void ChargerInterface::onWdogTimerExpire(uint32_t chargeSpeed)
{
	ChargeCurrent_low low;
	low.byte = (chargeSpeed << 6) & 0b11000000;

	ChargeCurrent_low high;
	high.byte = (chargeSpeed >> 3) & 0b00011111;

	bool valid = I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_3_LOW, &low.byte, 1);
	valid &= I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_3_HIGH, &high.byte, 1);
	if (!valid) {
		DLOG_WARNING("Watchdog timer failed to write the current.");
	}
}

}
}