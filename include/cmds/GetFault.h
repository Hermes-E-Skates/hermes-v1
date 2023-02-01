// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef GETFAULT_H_
#define GETFAULT_H_


#include "../core/CommonDefs.h"
#include "BluetoothCommand.h"
#include "BluetoothResponse.h"


namespace hermes {
namespace bt {

union FaultData_t
{
    struct
    {
        uint8_t BAT_WARNING : 1;
        uint8_t BAT_CRITICAL : 1;
        uint8_t BATTERY_DISCONNECT: 1;
        uint8_t CHARGE_ERR : 1;
        uint8_t MOTOR_ERR : 1;
        uint8_t IMU_ERR : 1;
        uint8_t SOFTWARE_ERR : 1;
        uint8_t CRITICAL_FAULT : 1;
    } bit;
    uint8_t byte = 0;
};


class GetFaultCmd : public BluetoothCommand
{
public:
    GetFaultCmd(uint8_t* const bytes, uint8_t len);

private:
    virtual bool decodeData(uint8_t* const bytes, uint8_t len) override;

    bool mValid = false;
    CmdId_t mCmdId = UNKNOWN_CMD;
};


class GetFaultResp : public BluetoothResponse
{
public:
    GetFaultResp(bool ack, uint8_t speed, uint8_t);
    void SetFaultStatus(FaultData_t fault);

private:
    virtual void encodeData(std::vector<uint8_t>& bytes) override;

    bool mValid = false;
    CmdId_t mCmdId = UNKNOWN_CMD;
    FaultData_t mFaultStatus;
};

}
}

#endif // GETFAULT_H_