// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef MOTORINTERFACE_H_
#define MOTORINTERFACE_H_


#include "../core/BaseApp.h"
#include <Servo.h>

namespace hermes {
namespace hw {

class MotorInterface : core::BaseApp {
public:
    MotorInterface(void);

    virtual bool init(void) override;
    virtual void loop(void) override;
    virtual void onCriticalFault(const core::CriticalFault& criticalFault) override;

	void setThrottleInput(uint8_t throttle);
	void setMaxSpeed(MaxSpeed_t maxSpeed);
	void setMaxAccel(MaxAccel_t maxAccel);
	void setPIDK1(uint8_t k1);
	MaxSpeed_t getMaxSpeed(void) const;
	MaxAccel_t getMaxAccel(void) const;
	void motorOn(void);
	void motorOff(void);
	void enableMotor(void);
	void disableMotor(void);
	float getSpeedKmh(void);

private:
	Servo ESC;
    MaxSpeed_t mMaxSpeed = UNLIMITED;
    MaxAccel_t mMaxAccel = NORMAL;
    bool mMotorOn = false;
    bool mMotorEnabled = false;
	bool mMotorPrimed = false;
    uint8_t mPwmSignal = 0;
    uint8_t mThrottle = 0;
	uint32_t mLastTimeUpdated = 0;
	uint32_t mLastTimer = 0;
	int8_t mHallCounter = 0;
	
	float mHallSpeedBuffer[10];
	float mHallSpeed = 0.0f;
    uint16_t mIntervalOfIncrease = 10;
    core::GenericObserver<MotorInterface> mHallEffectObserver;

    float mKp = 1.5; // Proportional gain for PID controller
    float mKi = 0.2; // Integral gain for PID controller
    float mKd = 5.0; // Derivative gain for PID controller
	uint8_t mK1 = 1.0; // Dynamic scaling value
    float mIntegral = 0; // Integral term for PID controller
    float mPreviousError = 0; // Error from previous loop iteration for PID controller
	core::GenericTimer<MotorInterface> mMotorEnTimer;

	void motorReadyTimer(uint32_t userdata);
	void onHallEffectStateChange(Pin_t pin, int16_t state);
	unsigned long mLastTime = 0;
};

}
}


#endif // MOTORINTERFACE_H_
