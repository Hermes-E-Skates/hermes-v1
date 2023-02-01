// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef HEARTBEAT_H_
#define HEARTBEAT_H_


#include "../core/CommonDefs.h"
#include "BluetoothCommand.h"
#include "BluetoothResponse.h"


namespace hermes {
namespace bt {

class HeartbeatCmd : public BluetoothCommand
{
public:
    HeartbeatCmd(uint8_t* const bytes, uint8_t len);

private:
    virtual bool decodeData(uint8_t* const bytes, uint8_t len) override;
};


class HeartbeatResp : public BluetoothResponse
{
public:
    HeartbeatResp(bool ack);

private:
    virtual void encodeData(std::vector<uint8_t>& bytes) override;
};

}
}

#endif // HEARTBEAT_H_