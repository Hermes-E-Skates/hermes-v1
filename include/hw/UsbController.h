// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef USBCONTROLLER_H_
#define USBCONTROLLER_H_


#include "../core/BaseApp.h"


namespace hermes {
namespace hw {

<<<<<<< HEAD

union PdStatus_t
{
	struct
	{
		uint8_t BCR_CFG : 5;
		uint8_t DATA_ROLE : 1;
		uint8_t RSVD1 : 1;
		uint8_t POWER_ROLE : 1;
		uint8_t RSVD2 : 1;
		uint8_t CONTRACT_STATE : 1;
		uint8_t RSVD3 : 3;
		uint8_t SINK_TX_RDY_STATUS : 1;
		uint8_t POLICY_ENGINE_STATE : 1;
		uint8_t PD_SPEC_SUPPORTED : 2;
		uint8_t PARTNER_PD_SPEC : 1;
		uint8_t UNCHUNKED : 1;
		uint8_t RSVD : 11;
	}bits;
	uint32_t data;
};

union EventStatus_t
{
	struct
	{
		uint8_t TYPE_C_ATTCH : 1;
		uint8_t TYPE_C_DISCON : 1;
		uint8_t PD_NEGOTIATED : 1;
		uint8_t POWER_ROLE_SWAP : 1;
		uint8_t DATA_ROLE_SWAP : 1;
		uint8_t VCON_SWAP : 1;
		uint8_t HARD_RST_RECEIVED : 1;
		uint8_t HARD_RESET_SENT : 1;
		uint8_t SOFT_RESET_SENT : 1;
		uint8_t CABLE_RESET_SENT : 1;
		uint8_t TYPE_C_ERR_RECOVERY_INITIATED: 1;
		uint8_t BCR_SOURCE_DISABLED_STATE: 1;
		uint8_t RSVD1: 15;
		uint8_t UNEXPECTED_VOLTAGE_ON_VBUS: 1;
		uint8_t VBUS_VOLTAGE_OUT_OF_RNG: 1;
		uint8_t RSVD: 1;
	}bits;
	uint32_t data;
};


class UsbController : core::BaseApp {
public:
	UsbController(void);

	bool init(void) override;
	void loop(void) override;
	void onCriticalFault(const core::CriticalFault& criticalFault) override;

	uint16_t getVoltage();
	uint16_t getCurrent();

private:
	const uint16_t BUS_VOLTAGE = 0x100D;
	const uint16_t PD_STATUS = 0x1008;
	const uint16_t TYPE_C_STATUS = 0x100C;
	const uint16_t CURRENT_PDO = 0x1010;
	const uint16_t SELECT_SINK_PDO = 0x1005;
	const uint16_t RESET_CMD = 0x0008;
	const uint16_t EVENT_STATUS = 0x1044;
	const uint8_t CYPD3177_I2C_ADDR = 0x08;

	core::GenericTimer<UsbController> mStatusTimer;
	core::GenericObserver<UsbController> mFaultPinWatcher;
	PdStatus_t mPdStatus;
	EventStatus_t mEventStatus;
	uint8_t mVbus;

	void onStatusTimerExpire(uint32_t userData);
	void onFaultPinChange(Pin_t, int16_t value);
=======
class UsbController {
public:


private:


>>>>>>> e09fda06d7e08a289f39b5ee8a5606873968e394
};

}
}


#endif // USBCONTROLLER_H_