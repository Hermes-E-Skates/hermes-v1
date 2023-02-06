// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef SETMODE_H_
#define SETMODE_H_


#include "../core/CommonDefs.h"
#include "BluetoothCommand.h"
#include "BluetoothResponse.h"


namespace hermes {
namespace bt {

class SetModeCmd : public BluetoothCommand
{
public:
    SetModeCmd(void);
    Control_t getControlMode(void) const;

private:
    virtual bool decode(const uint8_t* const bytes, uint8_t len) override;
    Control_t mControl;
};

class SetModeResp : public BluetoothResponse
{
public:
    SetModeResp(bool ack);

private:
    virtual void encodeData(std::vector<uint8_t>& bytes) override;
};

}
}
#endif // SETMODE_H_