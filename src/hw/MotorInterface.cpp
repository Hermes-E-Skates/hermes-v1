// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/hw/MotorInterface.h"


namespace hermes {
namespace hw {

MotorInterface::MotorInterface(void)
	: mWheelSpeedTimer(this, &MotorInterface::onTimerExpire)
{
	return;
}

bool MotorInterface::init(void)
{
	registerTimer(&mWheelSpeedTimer);
	return false;
}

void MotorInterface::loop(void)
{
    if (mMotorEnabled) {
        if (mMotorOn) {
            // Calculate error between desired speed setpoint and current speed of the motor
            float speed = (float)mPwmSignal / __UINT8_MAX__ * MAX_SPEED; // Convert PWM signal to speed (assuming linear relationship)
            float error = mThrottle - speed;

            // Compute PID output
            unsigned long current_time = millis();
            float dt = (current_time - mLastTimeUpdated) / 1000.0; // Time elapsed since last loop iteration, in seconds
            mIntegral += error * dt;
            float derivative = (error - mPreviousError) / dt;
            float output = mKp * error + mKi * mIntegral + mKd * derivative;
            mPreviousError = error;
            mLastTimeUpdated = current_time;

            // Convert PID output to PWM signal (assuming linear relationship)
            float throttle = output / MAX_SPEED * __UINT8_MAX__;
            if (throttle > mThrottle) {
                throttle = mThrottle; // Limit throttle to maximum value set by user
            } else if (throttle < 0) {
                throttle = 0; // Ensure throttle is positive
            }
            mPwmSignal = (uint8_t)throttle;
            analogWrite(ESC_PWM_PIN, mPwmSignal);
        } else {
            // If motor is OFF, write 0's
            analogWrite(ESC_PWM_PIN, 0);
        }
    }
}

void MotorInterface::onCriticalFault(const core::CriticalFault& criticalFault)
{
	return;
}

void MotorInterface::setThrottleInput(float throttle)
{
	mThrottle = throttle;
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
	if (!mWheelSpeedTimer.isEnabled()) {
		mWheelSpeedTimer.start(100, PERIODIC);
	}
	return;
}

void MotorInterface::motorOff(void)
{
	mMotorOn = false;
	mWheelSpeedTimer.stop();
	return;
}

void MotorInterface::enableMotor(void)
{
	mMotorEnabled = true;
	digitalWrite(Pin_t::MOT_EN, HIGH);
	return;
}

void MotorInterface::disableMotor(void)
{
	mMotorEnabled = false;
	digitalWrite(Pin_t::MOT_EN, LOW);
	return;
}

uint8_t MotorInterface::getSpeedKmh(void)
{
	return;
}

void MotorInterface::onTimerExpire(uint32_t userdata)
{
	uint16_t h1 = analogRead(MOTOR_SENSE_H1);
	uint16_t h2 = analogRead(MOTOR_SENSE_H2);
	uint16_t h3 = analogRead(MOTOR_SENSE_H3);
	return;
}

}
}