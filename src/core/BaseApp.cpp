// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/core/BaseApp.h"
#include "../../include/core/EventLoop.h"
#include "Wire.h"


namespace hermes {
namespace core {

// --------------------------------------------------------
// @brief: default constructor.
// @return: void.
// --------------------------------------------------------
BaseApp::BaseApp(void)
    : mFaultMsgHandler(this, &BaseApp::handleCriticalFaultMsg, CRITICAL_FAULT_MSG)
{
    if (EventLoop::getInstance()) {
        EventLoop::getInstance()->registerApp(this);
        if (EventLoop::getInstance()->isInitDone()) {
            baseStart();
        }
    }
}

// --------------------------------------------------------
// @brief: default destructor.
// @return: void.
// --------------------------------------------------------
BaseApp::~BaseApp(void)
{
    return;
}

// --------------------------------------------------------
// @brief: calls overriden init function and registers the
// critical fault message handler.
// @return: void.
// --------------------------------------------------------
void BaseApp::baseStart(void)
{

    Wire.begin();
    registerMessageHandler(&mFaultMsgHandler);
    init();
}

// --------------------------------------------------------
// @brief: checks for new events before calling the 
// overridden loop function. 
// @return: void.
// --------------------------------------------------------
void BaseApp::baseUpdate(void)
{
    checkWatchedPins();
    updateTimers();
    loop();
    mLastTimestamp = micros();
}


// --------------------------------------------------------
// @brief: time since the last loop call.
// @return: returns the delta time since the last loop.
// --------------------------------------------------------
uint16_t BaseApp::deltaTime(void) const
{
    return static_cast<uint16_t>(micros() - mLastTimestamp);
}

// --------------------------------------------------------
// @brief: raises a critical fault and triggers each apps
// handler.
// @param: fault - critical fault being generated.
// @return: void.
// --------------------------------------------------------
void BaseApp::generateCriticalFault(const CriticalFault fault) const
{
    messages::FaultMsg* criticalFaultMsg = new messages::FaultMsg(fault);
    sendMessage(criticalFaultMsg);
    delete criticalFaultMsg;
}

// --------------------------------------------------------
// @brief: sends a message to interrupt another app.
// @param: msg - pointer to the message event.
// @return: void.
// --------------------------------------------------------
void BaseApp::sendMessage(const messages::Message* msg) const
{
    MessageServer* mes = MessageServer::getInstance();
    if (mes) {
        mes->publishMessage(msg);
    }
}

// --------------------------------------------------------
// @brief: registers a timer to the event framework.
// @param: timer - pointer to the timer.
// @return: bool - returns true if registered.
// --------------------------------------------------------
bool BaseApp::registerTimer(BaseTimer* timer)
{
    TimerId_t id = timer->getTimerId();
    bool notAlreadyRegistered = (mTimers.find(id) == mTimers.end());

    if (notAlreadyRegistered)
    {
        mTimers.insert(std::make_pair(id, timer));
    }

    return notAlreadyRegistered;
}

// --------------------------------------------------------
// @brief: registers a pin observer to the event
// framework.
// @param: pinObserver - pointer to the pin observer.
// @return: bool - returns true if registered.
// --------------------------------------------------------
bool BaseApp::registerObserver(BaseObserver* pinObserver)
{
    mPinWatchers.insert(std::make_pair(pinObserver->getObserverId(), pinObserver));
    return true;
}

// --------------------------------------------------------
// @Brief: registers message handler to the message
// server.
// @Param: handler - pointer to the message handler.
// @Return: void.
// --------------------------------------------------------
void BaseApp::registerMessageHandler(BaseMsgHandler* msgHandler)
{
    MessageServer::getInstance()->registerListener(msgHandler);
}

// --------------------------------------------------------
// @Brief: unregisters a timer by matching it's ID.
// @Param: id - timer to remove from registry.
// @Return: void.
// --------------------------------------------------------
void BaseApp::unregisterTimer(TimerId_t id)
{
    mTimers.erase(id);
}

// --------------------------------------------------------
// @Brief: unregisters a pin observer by ID.
// @Param: id - observer to remove from registry.
// @Return: void.
// --------------------------------------------------------
void BaseApp::unregisterObserver(ObserverId_t id)
{
    mPinWatchers.erase(id);
}

// --------------------------------------------------------
// @Brief: unregisters a message handler from message
// server.
// @param: msgHandler - pointer to msg handler to remove.
// @Return: void.
// --------------------------------------------------------
void BaseApp::unregisterMessageHandler(BaseMsgHandler* msgHandler)
{
    MessageServer::getInstance()->unregisterListener(msgHandler);
}

// --------------------------------------------------------
// @Brief: goes through all timers and updates the 
// current time. If timer has elapsed, the callback will
// be triggered.
// @Return: void.
// --------------------------------------------------------
void BaseApp::updateTimers(void)
{
    auto it = mTimers.begin();
    for (it; it != mTimers.end(); it++)
    {
        TimerId_t id = it->first;
        BaseTimer* timer = it->second;

        if (timer->isEnabled())
        {
            if (timer->hasExpired())
            {
                if (timer->getType() == ONESHOT)
                {
                    timer->stop();
                }
                timer->executeCallback();
            }
        }
    }
}

// --------------------------------------------------------
// @Brief: checks each watched pin and triggers the
// callback if the state has changed.
// @Return: void.
// --------------------------------------------------------
void BaseApp::checkWatchedPins(void)
{
    auto it = mPinWatchers.begin();
    for (it; it != mPinWatchers.end(); it++)
    {
        ObserverId_t id = it->first;
        BaseObserver* pinWatcher = it->second;

        if (pinWatcher->isEnabled())
        {
            if (pinWatcher->hasPinChanged())
            {
                pinWatcher->executeCallback();
            }
        }
    }
}

// --------------------------------------------------------
// @Brief: message callback when Fault msg is generated.
// @param: msg - pointer to the fault message.
// @Return: void.
// --------------------------------------------------------
void BaseApp::handleCriticalFaultMsg(const messages::FaultMsg* msg)
{
    const CriticalFault& criticalFault = msg->getFaultObject();
    onCriticalFault(criticalFault);
}

}
}
