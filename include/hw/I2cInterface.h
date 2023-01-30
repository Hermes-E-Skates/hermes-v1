// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef I2CINTERFACE_H_
#define I2CINTERFACE_H_


namespace eskates {
namespace hw {

class I2cInterface {
public:
	static I2cInterface* getInstance(void);
	bool i2cWrite(uint8_t slaveAddr, uint8_t registerAddr, uint8_t* data, uint8_t len) const;
	bool i2cRead(uint8_t slaveAddr, uint8_t registerAddr, uint8_t* data, uint8_t len) const;

private:
	I2cInterface(void);
	static I2cInterface* mInstance;
};

}
}

#endif // I2CINTERFACE_H_