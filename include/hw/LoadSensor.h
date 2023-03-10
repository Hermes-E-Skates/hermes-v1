// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef LOADSENSOR_H_
#define LOADSENSOR_H_


#include "../core/BaseApp.h"
#include <HX711_ADC.h>


namespace hermes {
namespace hw {

class LoadSensor : public core::BaseApp {
public:
    enum LeanState_t: uint8_t{
        FORWARD,
        BACKWARD,
        NEUTRAL
    };
    virtual bool init(void) override;
	virtual void loop(void) override;
	virtual void onCriticalFault(const core::CriticalFault& criticalFault) override;

    LoadSensor(void);

    LeanState_t getLeanState(void);

private:
    const float CALIBRATION_VALUE1 = 696.0;
    const float CALIBRATION_VALUE2 = 733.0;

    HX711_ADC mLoadCellFront; //HX711 1
    HX711_ADC mLoadCellBack; //HX711 2

    LeanState_t mLeanState = NEUTRAL;
};

}
}


#endif // LOADSENSOR_H_