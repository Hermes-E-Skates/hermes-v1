// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/hw/I2cInterface.h"


namespace hermes {
namespace hw {

I2cInterface* I2cInterface::mInstance = nullptr;

I2cInterface::I2cInterface(void)
{
	Wire.begin();
	Wire.setClock(100000); // set clock 100khz
	Wire.setTimeout(100);
}

I2cInterface* I2cInterface::getInstance(void)
{
	if (mInstance != nullptr) {
		mInstance = new I2cInterface();
	}
	return mInstance;
}


bool I2cInterface::i2cWrite(uint8_t slaveAddr, uint8_t registerAddr, uint8_t* data, uint8_t len) const
{
    bool success = (len > 0) && (data != nullptr);
    if (success) {
        Wire.beginTransmission(slaveAddr);
        Wire.write(registerAddr);
        Wire.write(data, len);
    }

    success = (Wire.endTransmission() == 0);
    return success;
}

bool I2cInterface::i2cRead(uint8_t slaveAddr, uint8_t registerAddr, uint8_t* data, uint8_t len) const
{
    bool success = (len > 0) && (data != nullptr);
    if (success) {
        Wire.beginTransmission(slaveAddr);
        Wire.write(registerAddr);
        success = (Wire.endTransmission(false) == 0);

        if (success) {
            success = (Wire.requestFrom(slaveAddr, len) == len);

            if (success) {
                Wire.readBytes(data, len);
                success = (Wire.endTransmission() == 0);
            }
        }
    }

    return success;
}

}
}

