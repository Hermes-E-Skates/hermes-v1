// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef CHARGERINTERFACE_H_
#define CHARGERINTERFACE_H_


#include "bq25713_i2c_defs.h"
#include "../core/BaseApp.h"
#include "../core/GenericTimer.h"
#include "../core/MessageHandler.h"
#include "../cmds/GetChargeStatus.h"


namespace hermes {
namespace hw {

class ChargerInterface : public core::BaseApp {
public:
	ChargerInterface();
	virtual bool init() override;
	virtual void loop() override;
	virtual void onCriticalFault(const core::CriticalFault& criticalFault) override;

	void startCharging(void);
	void stopCharging(void);
	bool setChargeSpeed(ChargeSpeed_t chargeSpeed);
	uint8_t getInputCurrent(void) const;
	uint8_t getInputVoltage(void) const;
	bt::ChargeStatus_t getChargerStatus(void) const;

private:
	bool configureBq25713(void);
	void onWdogTimerExpire(uint32_t userData);

	const uint8_t BQ25713_I2C_ADDR = 0x6B;
	bool mCharging = false;
	ChargeSpeed_t mChargeSpeed;
	core::GenericTimer<ChargerInterface> mWatchdogTimer;
};

}
}

#endif // CHARGERINTERFACE_H_