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
		float output = mThrottle + mKp * error + mKi * mIntegral + mKd * derivative;

		mPreviousError = error;
		mLastTimeUpdated = current_time;

		uint8_t throttle = 0;
		// Convert PID output to PWM signal (assuming linear relationship)
		if (output > 0) {
			throttle = output * 180 / 63;
			//throttle = (uint8_t)(-0.0002 * pow(output, 4) + 0.0257 * pow(output, 3) - 1.4779 * pow(output, 2) + 39.866 * output - 356.35);
		}
		if (throttle > 180) {
			throttle = 180; // Limit throttle to maximum value set by user
		}
		else if (throttle < 0) {
			throttle = 0; // Ensure throttle is positive
		}
		mPwmSignal = (uint8_t)throttle;

		Serial.print("Out:\t");
		Serial.print(mThrottle);
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
	mThrottle = map(throttle, 0, 255, 0.0f, 180);

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
	// return mHallSpeed;
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
			//Serial.println(delta);
			mLastTime = millis();
			float rpm = (60000.0f / delta);
			mHallSpeedBuffer[(int)mHallCounter/7] = rpm * 12.5f * 0.001885f; // km/h
			Serial.println(rpm * 12.5f * 0.001885f);
			//mHallSpeed = rpm * 12.5f * 0.001885f; // km/h
		}
		if (mHallCounter >= 69) {
			mHallCounter = -1;
		}

		//uint32_t delta = micros() - mLastTime;
		////Serial.println(delta);
		//mLastTime = micros();
		//float rpm = (60000000.0f / delta);
		//mHallSpeedBuffer[mHallCounter] = rpm * 12.5f * 0.001885f; // km/h


		//unsigned long delta = millis() - mLastTime;
		//mLastTime = millis();
		//float rpm = 60000.0f * 1.0f / 7.0f / (float)delta;

		//mHallSpeedBuffer[mHallCounter % 10] = rpm * 0.0235619449f; // Convert RPM to KMH (wheel diamter is 0.125m)
		//mHallCounter++;

		//if (mHallCounter > 69) {
		//	mHallCounter = 0;
		//}
	}
}

}
}
