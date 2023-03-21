// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef BLUETOOTHCMD_H_
#define BLUETOOTHCMD_H_


#include "../core/CommonDefs.h"


namespace hermes {
namespace bt {

enum CmdId_t : int8_t {
    UNKNOWN_CMD = -0x01,
    HEARTBEAT = 0x20,
    GET_ID = 0x21,
    GET_INFO = 0x22,
    GET_FAULT = 0x23,
    GET_BATTERY = 0x24,
    GET_CHARGER = 0x25,
    GET_IMU = 0x26,
    SET_THROTTLE = 0x40,
    SET_MODE = 0x41,
    SET_MAX_SPEED = 0x42,
    SET_MAX_ACCEL = 0x43,
    SET_MOTOR_EN = 0x44,
    SET_PID_K1 = 0x45,
    CMD_ID_END = 0x46,
};

class BluetoothCommand
{
public:
    BluetoothCommand(CmdId_t id);
    CmdId_t getCmdId(void) const;
    bool valid(void) const;
    void setValid(bool valid);
	virtual bool decode(const uint8_t* const bytes, uint8_t len) = 0;

private:

    bool mValid = false;
    CmdId_t mCmdId = UNKNOWN_CMD;
};

}
}

#endif // BLUETOOTHCMD_H_