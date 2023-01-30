// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef MESSAGE_HANDLER_H_
#define MESSAGE_HANDLER_H_


#include "CommonDefs.h"
#include "../messages/ChargeStatusMsg.h"
#include "Arduino.h"


namespace eskates {
namespace core {

class BaseMsgHandler
{
public:
    virtual void executeCallback(const messages::Message* msg) = 0;
    virtual MsgId_t getMsgId(void) const = 0;
};

template<typename T, typename M>
class MsgHandler : public BaseMsgHandler {
public:
    typedef void(T::* MsgCallback)(const M* msg);
    MsgHandler(T* classPtr, MsgCallback callbackFunction, MsgId_t subscribedMsgId)
        : mClassPtr(classPtr)
        , mMsgCallback(callbackFunction)
        , mSubscribedMsgId(subscribedMsgId)
    {
        return;
    }

    ~MsgHandler()
    {
        return;
    }

    void executeCallback(const messages::Message* msg) override
    {
        (mClassPtr->*mMsgCallback)((const M*)(msg));
    }

    MsgId_t getMsgId(void) const override
    {
        return mSubscribedMsgId;
    }

private:
    T* mClassPtr;
    MsgCallback mMsgCallback;
    MsgId_t mSubscribedMsgId;
};
}
}

#endif // MESSAGE_HANDLER_H_