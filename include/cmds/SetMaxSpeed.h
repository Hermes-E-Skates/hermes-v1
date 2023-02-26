// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef SETMAXSPEED_H_
#define SETMAXSPEED_H_


#include "../core/CommonDefs.h"
#include "BluetoothCommand.h"
#include "BluetoothResponse.h"


namespace hermes {
namespace bt {

class SetMaxSpeedCmd : public BluetoothCommand
{
public:
    SetMaxSpeedCmd(void);
    MaxSpeed_t getMaxSpeed(void) const;

private:
    virtual bool decode(const uint8_t* const bytes, uint8_t len) override;
    MaxSpeed_t mMaxSpeed;
};

class SetMaxSpeedResp : public BluetoothResponse
{
public:
    SetMaxSpeedResp(bool ack);

private:
    virtual void encodeData(std::vector<uint8_t>& bytes) override;
};

}
}
#endif // SETMAXSPEED_H_