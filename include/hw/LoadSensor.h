// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef LOAD_SENSOR_H_
#define LOAD_SENSOR_H_

#include "../core/BaseApp.h"
#include <HX711_ADC.h>

namespace hermes {
namespace hw {

class LoadSensor {
public:
    LoadSensor(void);
    void init(void);
    void loop(void);
    float getThrottle(void);

private:
    const int HX711_dout_front = 4;
    const int HX711_sck_front = 5;
    const int HX711_dout_back = 6;
    const int HX711_sck_back = 7;
    HX711_ADC mLoadCellFront;
    HX711_ADC mLoadCellBack;
    const float mMaxLoad = 1000.0; // Maximum expected load in grams
    float mFrontLoad = 0.0;
    float mBackLoad = 0.0;
    float mThrottle = 0.0;
    unsigned long mLastUpdateTime = 0;
    const unsigned long mUpdateInterval = 100; // Time in milliseconds between updates

    void updateLoadValues(void);
    void calculateThrottle(void);
};

}
}

#endif // LOAD_SENSOR_H_
