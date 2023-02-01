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
    HEARTBEAT = 0x4A,
    GET_ID = 0x20,
    GET_INFO = 0x21,
    GET_FAULT = 0x22,
    GET_BATTERY = 0x23,
    GET_CHARGER = 0x24,
    GET_IMU = 0x25,
    SET_THROTTLE = 0x40,
    SET_MODE = 0x41,
    SET_MAX_SPEED = 0x42,
    SET_MAX_ACCEL = 0x43,
    CMD_ID_END = 0x44
};

class BluetoothCommand
{
public:
    BluetoothCommand(CmdId_t id, uint8_t* const data, uint8_t len);
    CmdId_t getCmdId(void) const;
    bool valid(void) const;

private:
	virtual bool decodeData(uint8_t* const bytes, uint8_t len) = 0;

    bool mValid = false;
    CmdId_t mCmdId = UNKNOWN_CMD;
};

}
}

#endif // BLUETOOTHCMD_H_