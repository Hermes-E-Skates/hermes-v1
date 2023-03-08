// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/hw/LoadSensor.h"
#include "../../include/core/DevLog.h"


namespace hermes {
namespace hw {

LoadSensor::LoadSensor()    
    : mLoadCell1(MOSI_PIN, GPIO_2_PIN)
    , mLoadCell2(MISO_PIN, SCK_PIN)
{
    return;
}

bool LoadSensor::init(void)
{
    DLOG_INFO("Init load sensor");

    mLoadCell1.begin();
    mLoadCell2.begin();
    const unsigned long STABILIZING_TIME = 1000; // tare preciscion can be improved by adding a few seconds of stabilizing time
    bool tare = true; //set this to false if you don't want tare to be performed in the next step
    byte loadcell1Rdy = 0;
    byte loadcell2Rdy = 0;
    while ((loadcell1Rdy + loadcell2Rdy) < 2) { //run startup, stabilization and tare, both modules simultaniously
    if (!loadcell1Rdy) loadcell1Rdy = mLoadCell1.startMultiple(STABILIZING_TIME, tare);
    if (!loadcell2Rdy) loadcell2Rdy = mLoadCell2.startMultiple(STABILIZING_TIME, tare);
    }
    if (mLoadCell1.getTareTimeoutFlag()) {
    DLOG_ERROR("Timeout, check MCU>HX711 no.1 wiring and pin designations");
    }
    if (mLoadCell2.getTareTimeoutFlag()) {
    DLOG_ERROR("Timeout, check MCU>HX711 no.2 wiring and pin designations")
    }
    mLoadCell1.setCalFactor(calibrationValue_1); // user set calibration value (float)
    mLoadCell2.setCalFactor(calibrationValue_2); // user set calibration value (float)
    DLOG_INFO("Startup is complete")
}

void LoadSensor::loop(void)
{
    const int serialPrintInterval = 0; //increase value to slow down serial print activity

    // check for new data/start next conversion
    // get smoothed value from data set
    if (mLoadCell1.update() || mLoadCell2.update()) {
        float a = -1*mLoadCell1.getData();
        float b = -1*mLoadCell2.getData();
        
        float forward_threshold = 30;
        float stopping_threshold = 40;

        if(a > forward_threshold & b < stopping_threshold){
            mLeanState = BACKWARD;
        }
        else if(b > forward_threshold & a < forward_threshold){
            mLeanState = FORWARD;
        }
        else{
            mLeanState = NEUTRAL;
        }
    }
}

void LoadSensor::onCriticalFault(const core::CriticalFault& criticalFault)
{
	return;
}

}
}