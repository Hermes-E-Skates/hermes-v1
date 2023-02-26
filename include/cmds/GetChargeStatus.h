// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef GETCHARGESTATUS_H_
#define GETCHARGESTATUS_H_


#include "../core/CommonDefs.h"
#include "BluetoothCommand.h"
#include "BluetoothResponse.h"


namespace hermes {
namespace bt {

union ChargeStatus_t
{
    struct __attribute__((packed))
    {
<<<<<<< HEAD
=======
        uint8_t STATE : 2; // FAULT DEAD_BATTERY READY CHARGING 
>>>>>>> e09fda06d7e08a289f39b5ee8a5606873968e394
        uint8_t INPUT_SOURCE_OK : 1;
        uint8_t IN_FAST_CHARGE : 1;
        uint8_t IN_PRE_CHARGE : 1;
        uint8_t INPUT_OVERVOLT : 1;
        uint8_t INPUT_OVERCURRENT : 1;
        uint8_t CHARGER_BATTERY_ERR : 1;
        uint8_t SYS_SHORT : 1;
        uint8_t RSVD : 1;
    } bit;
    uint8_t byte = 0;
};

class GetChargeStatusCmd : public BluetoothCommand
{
public:
    GetChargeStatusCmd(void);

private:
<<<<<<< HEAD
    virtual bool decode(const uint8_t* const bytes, uint8_t len) override;
=======
    virtual bool decode(uint8_t* const bytes, uint8_t len) override;
>>>>>>> e09fda06d7e08a289f39b5ee8a5606873968e394
};


class GetChargeStatusResp : public BluetoothResponse
{
public:
    GetChargeStatusResp(bool ack);
    void setChargeStatus(const ChargeStatus_t status);
    void setInputVoltage(uint8_t status);
    void setChargeCurrent(uint8_t status);

private:
    virtual void encodeData(std::vector<uint8_t>& bytes) override;

    ChargeStatus_t mChargeStatus;
    uint8_t mInputVoltage;
    uint8_t mInputCurrent;
};

}
}

#endif // GETCHARGESTATUS_H_