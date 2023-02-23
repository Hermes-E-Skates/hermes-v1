// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef GETBATTERY_H_
#define GETBATTERY_H_


#include "../core/CommonDefs.h"
#include "BluetoothCommand.h"
#include "BluetoothResponse.h"


namespace hermes {
namespace bt {

union BatteryStatus_t
{
    struct __attribute__((packed))
    {
        uint8_t IC_FAULT : 1;
        uint8_t EXTERNAL_DISABLE : 1;
        uint8_t UNDERVOLTAGE : 1;
        uint8_t OVERVOLTAGE : 1;
        uint8_t SHORTCIRCUIT : 1;
        uint8_t OVERCURRENT : 1;
        uint8_t RSVD : 2;
    } bit;
    uint8_t byte = 0;
};


class GetBatteryCmd : public BluetoothCommand
{
public:
    GetBatteryCmd(void);

private:
    virtual bool decode(uint8_t* const bytes, uint8_t len) override;
};


class GetBatteryResp : public BluetoothResponse
{
public:
    GetBatteryResp(bool ack);
    void setBatteryStatus(const BatteryStatus_t current);
    void setCellVoltage(uint16_t cell1, uint16_t cell2, uint16_t cell3);
    void setTemp(uint16_t temp);
    void setCurrent(uint16_t current);

private:
    virtual void encodeData(std::vector<uint8_t>& bytes) override;

    BatteryStatus_t mBatteryStatus;
    uint16_t mCell1;
    uint16_t mCell2;
    uint16_t mCell3;
    uint16_t mTemp;
    uint16_t mCurrent;
};

}
}

#endif // GETBATTERY_H_