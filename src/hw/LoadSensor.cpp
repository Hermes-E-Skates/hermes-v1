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
    : mLoadCellFront(MOSI_PIN, GPIO_2_PIN)
    , mLoadCellBack(MISO_PIN, SCK_PIN)
{
    return;
}

bool LoadSensor::init(void)
{
    // DLOG_INFO("Init load sensor");

    mLoadCellFront.begin();
    mLoadCellBack.begin();
    const unsigned long STABILIZING_TIME = 1000; // tare preciscion can be improved by adding a few seconds of stabilizing time
    bool tare = true; //set this to false if you don't want tare to be performed in the next step
    byte loadcell1Rdy = 0;
    byte loadcell2Rdy = 0;
    while ((loadcell1Rdy + loadcell2Rdy) < 2) { //run startup, stabilization and tare, both modules simultaniously
    if (!loadcell1Rdy) loadcell1Rdy = mLoadCellFront.startMultiple(STABILIZING_TIME, tare);
    if (!loadcell2Rdy) loadcell2Rdy = mLoadCellBack.startMultiple(STABILIZING_TIME, tare);
    }
    if (mLoadCellFront.getTareTimeoutFlag()) {
    // DLOG_ERROR("Timeout, check MCU>HX711 no.1 wiring and pin designations");
    }
    if (mLoadCellBack.getTareTimeoutFlag()) {
    // DLOG_ERROR("Timeout, check MCU>HX711 no.2 wiring and pin designations")
    }
    mLoadCellFront.setCalFactor(CALIBRATION_VALUE1); // user set calibration value (float)
    mLoadCellBack.setCalFactor(CALIBRATION_VALUE2); // user set calibration value (float)
    // DLOG_INFO("Startup is complete")
}

void LoadSensor::loop(void)
{
    /*const int serialPrintInterval = 0; //increase value to slow down serial print activity

    // check for new data/start next conversion
    // get smoothed value from data set
    if (mLoadCellFront.update() || mLoadCellBack.update()) {
        float front = -1*mLoadCellFront.getData();
        float back = -1*mLoadCellBack.getData();
        
        float forward_threshold = 30;
        float backward_threshold = 40;

        if(front > forward_threshold && back < backward_threshold) {
            mLeanState = FORWARD;
        }
        else if(back > backward_threshold && front < forward_threshold) {
            mLeanState = BACKWARD;
        }
        else {
            mLeanState = NEUTRAL;
        }
    }*/
}

void LoadSensor::onCriticalFault(const core::CriticalFault& criticalFault)
{
	return;
}

LoadSensor::LeanState_t LoadSensor::getLeanState(void)
{
    return mLeanState;
}

}
}