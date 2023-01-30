#include "../../include/hw/ChargerInterface.h"


namespace eskates {
namespace hw {

ChargerInterface::ChargerInterface()
	: mChargeStatusPoll(this, &ChargerInterface::onTimerExpire)
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
	return false;
}

bool ChargerInterface::clearFault(void)
{
	return false;
}

bool ChargerInterface::configureBq25713(void)
{
	return false;
}

void ChargerInterface::onTimerExpire(uint32_t userData)
{

}

}
}