// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef SETMAXACCEL_H_
#define SETMAXACCEL_H_


#include "../core/CommonDefs.h"
#include "BluetoothCommand.h"
#include "BluetoothResponse.h"


namespace hermes {
namespace bt {

class SetMaxAccelCmd : public BluetoothCommand
{
public:
    SetMaxAccelCmd(void);
    MaxAccel_t getMaxAccel(void) const;

private:
    virtual bool decode(const uint8_t* const bytes, uint8_t len) override;
    MaxAccel_t mMaxAccel;
};

class SetMaxAccelResp : public BluetoothResponse
{
public:
    SetMaxAccelResp(bool ack);

private:
    virtual void encodeData(std::vector<uint8_t>& bytes) override;
};

}
}
#endif // SETMAXACCEL_H_