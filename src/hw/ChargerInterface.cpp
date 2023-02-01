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
	: mStatusTimer(this, &ChargerInterface::onStatusTimerExpire)
	, mWatchdogTimer(this, &ChargerInterface::onWdogTimerExpire)
{
	return;
}

bool ChargerInterface::init()
{
	bool status = false;

	return status;
}

void ChargerInterface::loop()
{

}

void ChargerInterface::onCriticalFault(const core::CriticalFault& criticalFault)
{
}

bool ChargerInterface::startCharging(void)
{
	return false;
}

bool ChargerInterface::setChargeSpeed(ChargeSpeed_t chargeSpeed)
{
	if (chargeSpeed > 0 && chargeSpeed <= MAX_SPEED)
	{
		mWatchdogTimer.start(4000, PERIODIC, static_cast<uint32_t>(chargeSpeed));
	}
	return false;
}

bool ChargerInterface::clearFault(void)
{
	return false;
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
	I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_1_HIGH, &charge1low.byte, 1);

	ChargeOption1_high charge1high;
	charge1high.bit.EN_IBAT = 0;
	charge1high.bit.EN_PROCHOT_LPWR = 0b00;
	charge1high.bit.EN_PSYS = 1;
	charge1high.bit.RSNS_RAC = 0;
	charge1high.bit.RSNS_RSR = 0;
	charge1high.bit.PSYS_RATIO = 1;
	charge1high.bit.PTM_PINSEL = 0;
	I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_1_LOW, &charge1high.byte, 1);

	ChargeOption2_low charge2low;
	charge2low.bit.EN_EXTILIM = 0;
	charge2low.bit.EN_ICHG_IDCHG = 0;
	charge2low.bit.Q2_OCP = 1;
	charge2low.bit.ACX_OCP = 1;
	charge2low.bit.EN_ACOC = 1;
	charge2low.bit.ACOC_VTH = 1;
	charge2low.bit.EN_BATOC = 1;
	charge2low.bit.BATOC_VTH = 1;
	I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_2_HIGH, &charge2low.byte, 1);

	ChargeOption2_high charge2high;
	I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_2_LOW, &charge2high.byte, 1);
	ChargeOption3_low charge3low;
	ChargeOption3_high charge3high;

	I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_3_LOW, &charge3high.byte, 1);
	I2cInterface::getInstance()->i2cWrite(BQ25713_I2C_ADDR, CHARGE_OPTION_3_HIGH, &charge3low.byte, 1);

	
}

void ChargerInterface::onStatusTimerExpire(uint32_t userData)
{
	// Read input voltage
	// Read 
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