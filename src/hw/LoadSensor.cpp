// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/hw/LoadSensor.h"


namespace hermes {
namespace hw {

LoadSensor::LoadSensor(void) : mLoadCellFront(HX711_dout_front, HX711_sck_front), mLoadCellBack(HX711_dout_back, HX711_sck_back) {}

void LoadSensor::init(void) {
    mLoadCellFront.begin();
    mLoadCellBack.begin();
}

void LoadSensor::loop(void) {
    unsigned long currentTime = millis();
    if (currentTime - mLastUpdateTime >= mUpdateInterval) {
        mLastUpdateTime = currentTime;
        updateLoadValues();
        calculateThrottle();
    }
}

void LoadSensor::updateLoadValues(void) {
    // Read load cell values and convert to grams
    mFrontLoad = mLoadCellFront.getData() * mMaxLoad;
    mBackLoad = mLoadCellBack.getData() * mMaxLoad;
}

void LoadSensor::calculateThrottle(void) {
    // Calculate the difference in load between front and back
    float loadDiff = mFrontLoad - mBackLoad;

    // If load difference is within tolerance, maintain previous throttle
    if (abs(loadDiff) < 50.0) {
        return;
    }

    // Calculate the throttle based on the load difference
    mThrottle = map(loadDiff, -mMaxLoad, mMaxLoad, -255, 255);
    mThrottle = constrain(mThrottle, -255, 255);

    // Invert throttle if load is applied to back of skates
    if (loadDiff < 0.0) {
        mThrottle = -mThrottle;
    }
}

float LoadSensor::getThrottle(void) {
    return mThrottle;
}

}
}
