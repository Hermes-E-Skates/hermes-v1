// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/hw/MotorInterface.h"
#include "../../include/hw/LoadSensor.h"
#include "../../include/cmds/SetPIDK1.h"


namespace hermes {
namespace hw {

MotorInterface::MotorInterface(void)
	: mMotorEnTimer(this, &MotorInterface::motorReadyTimer)
	, mHallEffectObserver(this, &MotorInterface::onHallEffectStateChange, Pin_t::MOTOR_SENSE_H1)
{
	return;
}

bool MotorInterface::init(void)
{
	Serial.begin(115200); delay(10);
	registerTimer(&mMotorEnTimer);
	registerObserver(&mHallEffectObserver);

	Serial.println("Successfully registered the hall effect sensor.");
	ESC.attach(ESC_PWM_PIN, 1000, 2000);

	return false;
}

void MotorInterface::loop(void)
{
	if (Serial.read() > 0) {
		mKp += 0.1;
		Serial.print("############# Kp = ");
		Serial.println(mKp);
	}
	if (mMotorOn && mMotorPrimed) {
		// Calculate error between desired speed setpoint and current speed of the motor
		if (millis() - mLastTime > 282) {
			for (int i = 0; i < 10; i++) {
				mHallSpeedBuffer[i] = 0; // kmph
			}
			Serial.println("FLUSHING BUFFER");
			mLastTime = millis();
			mHallCounter = 0;
		}

		float speed = MotorInterface::getSpeedKmh();
		float error = mThrottle - speed;

		Serial.print("In:\t");
		Serial.print(mThrottle);
		Serial.print("\t");

		Serial.print("Hall:\t");
		Serial.print(speed);
		Serial.print("\t");


		Serial.print("E:\t");
		Serial.print(error);
		Serial.print("\t");

		// Compute PID output
		unsigned long current_time = millis();
		float dt = (current_time - mLastTimeUpdated) / 1000.0f; // Time elapsed since last loop iteration, in seconds
		mIntegral += error * dt;
		float derivative = (error - mPreviousError) / dt;
		

		float K1 = mK1 * 10.0f / 255.0;
		if (K1 < 1) {
			K1 = 1.0;
		}
		else if (K1 > 10) {
			K1 = 10.0;
		}

		Serial.print("K1:\t");
		Serial.print(K1);
		Serial.print("\t");

		float output = (mThrottle + mKp * error + mKi * mIntegral + mKd * derivative) * (float)K1;

		mPreviousError = error;
		mLastTimeUpdated = current_time;

		uint8_t throttle = 0;
		// Convert PID output to PWM signal (assuming linear relationship)
		if (output > 0) {
			throttle = output * 180 / 63;
		}
		if (throttle > 180) {
			throttle = 180; // Limit throttle to maximum value set by user
		}
		else if (throttle < 0) {
			throttle = 0; // Ensure throttle is positive
		}
		mPwmSignal = (uint8_t)throttle;

		Serial.print("Out:\t");
		Serial.print(throttle);
		Serial.print("\n");


		ESC.write(throttle); // throttle for PID, mThrottle without
	}
	else {
		ESC.write(0);
	}
}

void MotorInterface::onCriticalFault(const core::CriticalFault& criticalFault)
{
	return;
}

void MotorInterface::setThrottleInput(uint8_t throttle)
{
	mThrottle = map(throttle, 0, 255, 0.0f, 18);

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

void MotorInterface::setPIDK1(uint8_t k1)
{
	mK1 = k1;
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

	if (!mHallEffectObserver.isEnabled()) {
		mHallEffectObserver.enable();
		Serial.println("HallEffectObserver registered.");
	}

	return;
}

void MotorInterface::motorOff(void)
{
	mMotorOn = false;

	return;
}

void MotorInterface::enableMotor(void)
{
	mMotorEnabled = true;
	digitalWrite(Pin_t::MOT_EN, HIGH);
	mMotorEnTimer.start(1000, ONESHOT, 0);

	return;
}

void MotorInterface::disableMotor(void)
{
	mMotorEnabled = false;
	mMotorPrimed = false;
	digitalWrite(Pin_t::MOT_EN, LOW);

	return;
}

float MotorInterface::getSpeedKmh(void)
{
	float sum = 0;
	for (int i = 0; i < 10; i++) {
		sum += mHallSpeedBuffer[i];
	}
	if (sum <= 0) {
		return 0.0f;
	}
	return sum / 10.0f;
}

void MotorInterface::motorReadyTimer(uint32_t userdata)
{
	mMotorPrimed = true;
}

void MotorInterface::onHallEffectStateChange(Pin_t pin, int16_t state)
{
	if (state == 1) {
		mHallCounter++;
		uint32_t delta = millis() - mLastTime;

		if (mHallCounter % 7 == 0) {
			mLastTime = millis();
			float rpm = (60000.0f / delta);
			mHallSpeedBuffer[(int)mHallCounter/7] = rpm * 12.5f * 0.001885f; // km/h
		}
		if (mHallCounter >= 69) {
			mHallCounter = -1;
		}
	}
}

}
}
