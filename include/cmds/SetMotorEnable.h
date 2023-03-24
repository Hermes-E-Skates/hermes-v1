// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef SETMOTORENABLE_H_
#define SETMOTORENABLE_H_


#include "../core/CommonDefs.h"
#include "BluetoothCommand.h"
#include "BluetoothResponse.h"


namespace hermes {
namespace bt {

class SetMotorEnableCmd : public BluetoothCommand
{
public:
    SetMotorEnableCmd(void);
    bool isMotorEnabled(void) const;
    bool isMotorOn(void) const;
    bool isPwmForce(void) const;

private:
    virtual bool decode(const uint8_t* const bytes, uint8_t len) override;
    bool mMotorEnable;
    bool mMotorOn;
    bool mPwmForce;
};

class SetMotorEnableResp : public BluetoothResponse
{
public:
    SetMotorEnableResp(bool ack);

private:
    virtual void encodeData(std::vector<uint8_t>& bytes) override;
};

}
}
#endif // GETINFO_H_