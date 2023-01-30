// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef CHARGERINTERFACE_H_
#define CHARGERINTERFACE_H_


#include "../core/BaseApp.h"
#include "../core/GenericTimer.h"
#include "../core/MessageHandler.h"


namespace eskates {
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
	const uint8_t BQ25713_I2C_ADDR = 0x6B;

	bool configureBq25713(void);

	void onTimerExpire(uint32_t userData);

	uint32_t mChargeStatus;
	core::GenericTimer<ChargerInterface> mChargeStatusPoll;
};

}
}

#endif // CHARGERINTERFACE_H_