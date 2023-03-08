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
	MaxSpeed_t getMaxSpeed(void) const;
	MaxAccel_t getMaxAccel(void) const;
	void motorOn(void);
	void motorOff(void);
	void enableMotor(void);
	void disableMotor(void);
	uint8_t getSpeedKmh(void);

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
	uint16_t mIntervalOfIncrease = 10;
	core::GenericTimer<MotorInterface> mWheelSpeedTimer;
	core::GenericTimer<MotorInterface> mMotorEnTimer;

	void onTimerExpire(uint32_t userdata);
	void motorReadyTimer(uint32_t userdata);
};

}
}


#endif // MOTORINTERFACE_H_