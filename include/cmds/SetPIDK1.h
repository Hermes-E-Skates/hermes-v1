// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef SETPIDK1_H_
#define SETPIDK1_H_


#include "../core/CommonDefs.h"
#include "BluetoothCommand.h"
#include "BluetoothResponse.h"


namespace hermes {
namespace bt {

class SetPIDK1Cmd : public BluetoothCommand
{
public:
    SetPIDK1Cmd(void);
    uint8_t getPIDK1(void);

private:
    virtual bool decode(const uint8_t* const bytes, uint8_t len) override;
    uint8_t mK1 = 1;
};

class SetPIDK1Resp : public BluetoothResponse
{
public:
    SetPIDK1Resp(bool ack);

private:
    virtual void encodeData(std::vector<uint8_t>& bytes) override;
};

}
}
#endif // SETPIDK1_H_