// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef MOTORINTERFACE_H_
#define MOTORINTERFACE_H_


#include "../core/BaseApp.h"


namespace hermes {
namespace hw {

class MotorInterface {
public:
<<<<<<< HEAD
	void setThrottleInput(float throttle);
	void setMaxSpeed(MaxSpeed_t maxSpeed);
	void setMaxAccel(MaxAccel_t maxAccel);
	MaxSpeed_t getMaxSpeed(void) const;
	MaxAccel_t getMaxAccel(void) const;
	void enableMotor(void);
	void disableMotor(void);

private:
	MaxSpeed_t mMaxSpeed = UNLIMITED;
	MaxAccel_t mMaxAccel = NORMAL;
	bool mMotorEnabled = false;
=======


private:


>>>>>>> e09fda06d7e08a289f39b5ee8a5606873968e394
};

}
}


#endif // MOTORINTERFACE_H_