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
    HeartbeatCmd(void);

private:
<<<<<<< HEAD
    virtual bool decode(const uint8_t* const bytes, uint8_t len) override;
=======
    virtual bool decode(uint8_t* const bytes, uint8_t len) override;
>>>>>>> e09fda06d7e08a289f39b5ee8a5606873968e394
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