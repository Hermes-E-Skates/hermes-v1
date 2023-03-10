// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/hw/MotorInterface.h"
#include "../../include/hw/LoadSensor.h"


namespace hermes {
namespace hw {

MotorInterface::MotorInterface(void)
	: mThrottleUpdateTimer(this, &MotorInterface::onTimerExpire)
	, mMotorEnTimer(this, &MotorInterface::motorReadyTimer)
{
	return;
}

bool MotorInterface::init(void)
{
	registerTimer(&mThrottleUpdateTimer);
	registerTimer(&mMotorEnTimer);
	ESC.attach(ESC_PWM_PIN, 1000, 2000);
	return false;
}

void MotorInterface::loop(void)
{
	//if (mMotorEnabled) {
	//	if (mMotorOn) {
	//		// Algorithm needed

	//		// 

	//		// 

	//		// Ratio of throttle to speed
	//		float ratio = mPwmSignal / mThrottle;

	//		// Check the time
	//		uint32_t time = millis();
	//		if (time - mLastTimeUpdated > mIntervalOfIncrease) {
	//			mLastTimeUpdated = time;
	//			if (mPwmSignal < __UINT8_MAX__) {
	//				mPwmSignal++;
	//			}
	//		}

	//		// Write signal to motor
	//		analogWrite(ESC_PWM_PIN, mPwmSignal);
	//	} else {

	//		// If motor is OFF write 0's
	//		analogWrite(ESC_PWM_PIN, 0);
	//	}
	//}

	if (mMotorOn && mMotorPrimed) {
		ESC.write(mThrottle);
	} else {
		ESC.write(0);
	}
}

void MotorInterface::onCriticalFault(const core::CriticalFault& criticalFault)
{
	return;
}

void MotorInterface::setThrottleInput(uint8_t throttle)
{
	mThrottle = map(throttle, 0, 255, 0, 180);
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
	switch (mMaxSpeed) {

	case SLOW:
		mIntervalOfIncrease = 20;
		break;
	case NORMAL:
		mIntervalOfIncrease = 10;
		break;
	case FAST:
		mIntervalOfIncrease = 5;
		break;
	case RACE:
		mIntervalOfIncrease = 0;
		break;
	}
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

void MotorInterface::motorOn(void)
{
	mMotorOn = true;
	if (!mThrottleUpdateTimer.isEnabled()) {
		mThrottleUpdateTimer.startMicros(19600, PERIODIC);
	}
	return;
}

void MotorInterface::motorOff(void)
{
	mMotorOn = false;
	mThrottleUpdateTimer.stop();
	return;
}

void MotorInterface::enableMotor(void)
{
	mMotorEnabled = true;
	digitalWrite(Pin_t::MOT_EN, HIGH);
	mMotorEnTimer.start(7000, ONESHOT, 0);
	return;
}

void MotorInterface::disableMotor(void)
{
	mMotorEnabled = false;
	mMotorPrimed = false;
	digitalWrite(Pin_t::MOT_EN, LOW);
	return;
}

uint8_t MotorInterface::getSpeedKmh(void)
{
	return;
}

void MotorInterface::onTimerExpire(uint32_t userdata)
{
	if (mThrottle > mPwmSignal) {
		mPwmSignal++;
	} else if (mThrottle < mPwmSignal) {
		mPwmSignal -= 10;
	}

	return;
}

void MotorInterface::motorReadyTimer(uint32_t userdata)
{
	//if (userdata == 0) {
	//	mThrottle = 0;
	//	mMotorEnTimer.start(5000, ONESHOT, 1);
	//} else if (userdata == 1) {
	//	mThrottle = 0;
	//}
	mMotorPrimed = true;
}

}
}
