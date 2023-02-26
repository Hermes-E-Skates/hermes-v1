// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef SETTHROTTLE_H_
#define SETTHROTTLE_H_


#include "../core/CommonDefs.h"
#include "BluetoothCommand.h"
#include "BluetoothResponse.h"


namespace hermes {
namespace bt {

class SetThrottleCmd : public BluetoothCommand
{
public:
    SetThrottleCmd(void);
    float getPercentage(void) const;

private:
    virtual bool decode(const uint8_t* const bytes, uint8_t len) override;
    uint8_t mThrottle;
};

class SetThrottleResp : public BluetoothResponse
{
public:
    SetThrottleResp(bool ack);

private:
    virtual void encodeData(std::vector<uint8_t>& bytes) override;
};

}
}
#endif // GETINFO_H_