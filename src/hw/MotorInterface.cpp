// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/hw/MotorInterface.h"


namespace hermes {
namespace hw {

void MotorInterface::setThrottleInput(float throttle)
{
	// Algorithm needed
	return;
}

void MotorInterface::setMaxSpeed(MaxSpeed_t maxSpeed)
{
	mMaxSpeed = maxSpeed;
	return;
}

void MotorInterface::setMaxAccel(MaxAccel_t maxAccel)
{
	mMaxAccel = maxAccel;
	return;
}

MaxSpeed_t MotorInterface::getMaxSpeed(void) const
{
	return mMaxSpeed;
}

MaxAccel_t MotorInterface::getMaxAccel(void) const
{
	return mMaxAccel;
}

void MotorInterface::enableMotor(void)
{
	mMotorEnabled = true;
	digitalWrite(Pin_t::MOT_EN, HIGH);
}

void MotorInterface::disableMotor(void)
{
	mMotorEnabled = false;
	digitalWrite(Pin_t::MOT_EN, LOW);
}

}
}