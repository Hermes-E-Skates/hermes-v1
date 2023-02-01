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


namespace hermes {
namespace hw {

class ChargerInterface : public core::BaseApp {
public:
	ChargerInterface();

	virtual bool init() override;
	virtual void loop() override;
	virtual void onCriticalFault(const core::CriticalFault& criticalFault) override;

	bool startCharging(void);
	bool disableCharging(void);
	bool getFaultStatus(void) const;
	bool setChargeSpeed(ChargeSpeed_t chargeSpeed);
	bool clearFault(void);


private:
	bool configureBq25713(void);
	void onStatusTimerExpire(uint32_t userData);
	void onWdogTimerExpire(uint32_t userData);

	const uint8_t BQ25713_I2C_ADDR = 0x6B;
	bool mCharging = false;
	core::GenericTimer<ChargerInterface> mStatusTimer;
	core::GenericTimer<ChargerInterface> mWatchdogTimer;
};

}
}

#endif // CHARGERINTERFACE_H_