// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef GETINFO_H_
#define GETINFO_H_


#include "../core/CommonDefs.h"
#include "BluetoothCommand.h"
#include "BluetoothResponse.h"


namespace hermes {
namespace bt {

union SysStatus_t
{
    struct __attribute__((packed))
    {
        uint8_t FAULT_FLAG : 1;
        uint8_t RUN_STATE : 3;
        uint8_t BATTERY_STATE : 2;
        uint8_t RSVD : 2;
    } bit;
    uint8_t byte = 0;
};

union ConfigInfo_t
{
    struct __attribute__((packed))
    {
        Control_t control : 2;
        MaxSpeed_t maxSpeed : 3;
        MaxAccel_t maxAccel : 3;
    } bit;
    uint8_t byte = 0;
};


class GetInfoCmd : public BluetoothCommand
{
public:
    GetInfoCmd(void);

private:
    virtual bool decode(const uint8_t* const bytes, uint8_t len) override;
};


class GetInfoResp : public BluetoothResponse
{
public:
    GetInfoResp(bool ack);
    void setSysStatus(const SysStatus_t status);
    void setSpeedInfo(uint8_t speed);
    void setConfigInfo(const ConfigInfo_t config);

private:
    virtual void encodeData(std::vector<uint8_t>& bytes) override;

    SysStatus_t mSysStatus;
    uint8_t mSpeed;
    ConfigInfo_t mConfigInfo;
};

}
}
#endif // GETINFO_H_