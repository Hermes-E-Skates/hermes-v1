// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef BLUETOOTHCOMMANDHANDLER_H_
#define BLUETOOTHCOMMANDHANDLER_H_


#include "../core/CommonDefs.h"
#include "BluetoothCommand.h"
#include "BluetoothResponse.h"


namespace eskates {
namespace bt {

class BaseBluetoothHandler
{
public:
    virtual CmdId_t getCmdId(void) const = 0;
    virtual BluetoothResponse* handleCommand(const BluetoothCommand* cmd) const = 0;
};

template<typename T, typename M>
class GenericBluetoothHandler : public BaseBluetoothHandler {
public:
    typedef BluetoothResponse* (T::* CommandCallback)(const M*);
    GenericBluetoothHandler(T* classPtr, CommandCallback callbackFunction, CmdId_t id)
        : mClassPtr(classPtr)
        , mOnCmdReceived(callbackFunction)
        , mCmdId(id)
        , mEnabled(false)
    {
        return;
    }

    ~GenericBluetoothHandler(void)
    {
        return;
    }

    CmdId_t getCmdId(void) const override
    {
        return mCmdId;
    }

    virtual BluetoothResponse* handleCommand(const BluetoothCommand* cmd) const override
    {
        return (mClassPtr->*mOnCmdReceived)((const M*)(cmd));
    }

private:
    T* mClassPtr;
    CommandCallback mOnCmdReceived;
    CmdId_t mCmdId;
    bool mEnabled;
};

}
}
#endif // BLUETOOTHCOMMANDHANDLER_H_