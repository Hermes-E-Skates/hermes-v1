// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/hw/UsbController.h"
<<<<<<< HEAD
#include "../../include/hw/I2cInterface.h"
#include "../../include/core/DevLog.h"
=======
>>>>>>> e09fda06d7e08a289f39b5ee8a5606873968e394


namespace hermes {
namespace hw {

<<<<<<< HEAD
UsbController::UsbController(void)
	: BaseApp()
	, mStatusTimer(this, &UsbController::onStatusTimerExpire)
	, mFaultPinWatcher(this, &UsbController::onFaultPinChange, Pin_t::USBC_FAULT_PIN)
{
	return;
}

bool UsbController::init(void)
{
	bool status = registerTimer(&mStatusTimer);
	status &= registerObserver(&mFaultPinWatcher);

	mStatusTimer.start(1000, PERIODIC);
	mFaultPinWatcher.enable();

	uint8_t resetCmd[] = { 'R', 0b00000001 };
	if (I2cInterface::getInstance()->i2cWrite(CYPD3177_I2C_ADDR, RESET_CMD, resetCmd, 2))
	{
		DLOG_DEBUG("reset usb c ctrl successfully");
	}
	else {
		DLOG_DEBUG("Err: reset usb c ctrl failed");
	}

	if (I2cInterface::getInstance()->i2cWrite(CYPD3177_I2C_ADDR, RESET_CMD, resetCmd, 2))
	{
		DLOG_DEBUG("reset usb c ctrl successfully");
	}
	else {
		DLOG_DEBUG("Err: reset usb c ctrl failed");
	}



	uint8_t data = 0b11111111;
	if (I2cInterface::getInstance()->i2cWrite(CYPD3177_I2C_ADDR, SELECT_SINK_PDO, &data, 1))
	{
		DLOG_DEBUG("cfg usb c pd successfully");
	}
	else {
		DLOG_DEBUG("Err: cfg usb c pd failed");
	}
	return status;
}

void UsbController::loop(void)
{
	return;
}

void UsbController::onCriticalFault(const core::CriticalFault& criticalFault)
{
	return;
}

uint16_t UsbController::getVoltage()
{
	return uint16_t();
}

uint16_t UsbController::getCurrent()
{
	return uint16_t();
}

void UsbController::onStatusTimerExpire(uint32_t userData)
{
	bool valid = I2cInterface::getInstance()->i2cRead(
		CYPD3177_I2C_ADDR, PD_STATUS, reinterpret_cast<uint8_t*>(&mPdStatus.data), sizeof(PdStatus_t));
	
	valid &= I2cInterface::getInstance()->i2cRead(
		CYPD3177_I2C_ADDR, BUS_VOLTAGE, &mVbus, 1);

	valid &= I2cInterface::getInstance()->i2cRead(
		CYPD3177_I2C_ADDR, EVENT_STATUS, reinterpret_cast<uint8_t*>(&mEventStatus.data), sizeof(EventStatus_t));

	DLOG_INFO("VBUS voltage is %d", mVbus);
	DLOG_INFO("Fault pin is %s", USBC_FAULT_PIN ? "HIGH" : "LOW");
	//DLOG_INFO("BCR_CFG=0x%x DATA_ROLE=0x%x POWER_ROLE=0x%x CONTRACT_STATE=0x%x SINK_TX_RDY_STATUS=0x%x POLICY_ENGINE_STATE=0x%x PD_SPEC=0x%x PARTNER PD_SPEC=0x%x UNCHUNKED=0x%x"
	//	, mPdStatus.bits.BCR_CFG, mPdStatus.bits.DATA_ROLE, mPdStatus.bits.POWER_ROLE, mPdStatus.bits.CONTRACT_STATE
	//	, mPdStatus.bits.SINK_TX_RDY_STATUS, mPdStatus.bits.POLICY_ENGINE_STATE, mPdStatus.bits.PD_SPEC_SUPPORTED, mPdStatus.bits.PARTNER_PD_SPEC, mPdStatus.bits.UNCHUNKED);


	//DLOG_INFO("HardReset Received=%d Hard Reset Sent=%d Soft Reset Sent=%d Cable Reset Sent=%d Type-C Error Recovery Initiated=%d BCR Entered Source Disabled State=%d Unexpected Voltage on VBUS=%d VBUS out of bounds=%d"
	//	,mEventStatus.bits.HARD_RST_RECEIVED, mEventStatus.bits.HARD_RESET_SENT, mEventStatus.bits.SOFT_RESET_SENT, mEventStatus.bits.CABLE_RESET_SENT, mEventStatus.bits.TYPE_C_ERR_RECOVERY_INITIATED, mEventStatus.bits.BCR_SOURCE_DISABLED_STATE, mEventStatus.bits.UNEXPECTED_VOLTAGE_ON_VBUS, mEventStatus.bits.VBUS_VOLTAGE_OUT_OF_RNG);


	//DLOG_INFO("Type-C Device Attached=%d Type-C Device Disconnected=%d PD Contract Negotiated=%d PowerRoleSwap completed=%d Data Role Swap Completed=%d VCONN Swap Completed=%d"
	//	, mEventStatus.bits.TYPE_C_ATTCH, mEventStatus.bits.TYPE_C_DISCON, mEventStatus.bits.PD_NEGOTIATED, mEventStatus.bits.POWER_ROLE_SWAP, mEventStatus.bits.DATA_ROLE_SWAP, mEventStatus.bits.VCON_SWAP);
	
	// Write the same bits back to clear them.
	valid &= I2cInterface::getInstance()->i2cWrite(CYPD3177_I2C_ADDR, EVENT_STATUS, reinterpret_cast<uint8_t*>(&mEventStatus.data), sizeof(EventStatus_t));
	//DLOG_DEBUG("ValidI2c=%s", valid ? "true" : "false");

	return;
}

void UsbController::onFaultPinChange(Pin_t pin, int16_t value)
{
	DLOG_INFO("usb c FAULT pin changed [%s]", value == 0x01 ? "FAULT" : "NO FAULT");
	return;
}
=======
>>>>>>> e09fda06d7e08a289f39b5ee8a5606873968e394

}
}