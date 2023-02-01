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
        uint8_t RUN_STATE : 1;
        uint8_t BATTERY_STATE : 1;
        uint8_t RSVD : 5;
    } bit;
    uint8_t byte = 0;
};

union SpeedInfo_t
{
    struct __attribute__((packed))
    {
        uint8_t SPEED : 8;
    } bit;
    uint8_t byte = 0;
};

union BatteryInfo_t
{
    struct __attribute__((packed))
    {
        uint8_t BATTERY : 7;
        uint8_t RSVD : 1;
    } bit;
    uint8_t byte = 0;
};

union ConfigInfo_t
{
    struct __attribute__((packed))
    {
        uint8_t control : 2;
        uint8_t maxSpeed : 3;
        uint8_t maxAccel : 3;
    } bit;
    uint8_t byte = 0;
};


class GetInfoCmd : public BluetoothCommand
{
public:
    GetInfoCmd(uint8_t* const bytes, uint8_t len);

private:
    virtual bool decodeData(uint8_t* const bytes, uint8_t len) override;
};


class GetInfoResp : public BluetoothResponse
{
public:
    GetInfoResp(bool ack);
    void setSysStatus(const SysStatus_t fault);
    void setSpeedInfo(const SpeedInfo_t fault);
    void setBatteryInfo(const BatteryInfo_t fault);
    void setConfigInfo(const ConfigInfo_t fault);

private:
    virtual void encodeData(std::vector<uint8_t>& bytes) override;

    SysStatus_t mSysStatus;
    SpeedInfo_t mSpeedInfo;
    BatteryInfo_t mBatteryInfo;
    ConfigInfo_t mConfigInfo;
};

}
}

#endif // GETINFO_H_