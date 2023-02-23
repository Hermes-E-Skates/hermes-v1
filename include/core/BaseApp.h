// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef BASEAPP_H_
#define BASEAPP_H_


#include <map>
#include <functional>
#include "GenericTimer.h"
#include "GenericObserver.h"
#include "MessageServer.h"
#include "CriticalFault.h"
#include "MessageHandler.h"
#include "../messages/FaultMsg.h"


namespace hermes {
namespace core {

class BaseApp {
public:
	// --------------------------------------------------------
	// @brief: default constructor.
	// @return: void.
	// --------------------------------------------------------
	BaseApp(void);

	// --------------------------------------------------------
	// @brief: default destructor.
	// @return: void.
	// --------------------------------------------------------
	~BaseApp(void);

	// hide copy constructor
	BaseApp(const BaseApp&) = delete;

	// hide copy assignment constructor
	BaseApp& operator=(const BaseApp&) = delete;

	// --------------------------------------------------------
	// @brief: calls overriden init function and registers the
	// critical fault message handler.
	// @return: void.
	// --------------------------------------------------------
	void baseStart(void);

	// --------------------------------------------------------
	// @brief: checks for new events before calling the 
	// overridden loop function. 
	// @return: void.
	// --------------------------------------------------------
	void baseUpdate(void);

	// --------------------------------------------------------
	// @brief: *MUST OVERRIDE* - called on startup.
	// @return: bool - returns true if init was sucessful.
	// --------------------------------------------------------
	virtual bool init(void) = 0;

	// --------------------------------------------------------
	// @brief: *MUST OVERRIDE* - called repeately.
	// @return: void.
	// --------------------------------------------------------
	virtual void loop(void) = 0;

	// --------------------------------------------------------
	// @brief: *MUST OVERRIDE* - called when critical fault is
	// raised.
	// @param: criticalFault - reference to the fault object.
	// @return: void.
	// --------------------------------------------------------
	virtual void onCriticalFault(const CriticalFault& criticalFault) = 0;

protected:
	// --------------------------------------------------------
	// @brief: time since the last loop call.
	// @return: returns the delta time since the last loop.
	// --------------------------------------------------------
	uint16_t deltaTime(void) const;

	// --------------------------------------------------------
	// @brief: raises a critical fault and triggers each apps
	// handler.
	// @param: fault - critical fault being generated.
	// @return: void.
	// --------------------------------------------------------
	void generateCriticalFault(const CriticalFault fault) const;

	// --------------------------------------------------------
	// @brief: sends a message to interrupt another app.
	// @param: msg - pointer to the message event.
	// @return: void.
	// --------------------------------------------------------
	void sendMessage(const messages::Message* msg) const;

	// --------------------------------------------------------
	// @brief: registers a timer to the event framework.
	// @param: timer - pointer to the timer.
	// @return: bool - returns true if registered.
	// --------------------------------------------------------
	bool registerTimer(BaseTimer* timer);

	// --------------------------------------------------------
	// @brief: registers a pin observer to the event
	// framework.
	// @param: pinObserver - pointer to the pin observer.
	// @return: bool - returns true if registered.
	// --------------------------------------------------------
	bool registerObserver(BaseObserver* pinObserver);

	// --------------------------------------------------------
	// @Brief: registers message handler to the message
	// server.
	// @Param: handler - pointer to the message handler.
	// @Return: void.
	// --------------------------------------------------------
	void registerMessageHandler(BaseMsgHandler* handler);

	// --------------------------------------------------------
	// @Brief: unregisters a timer by matching it's ID.
	// @Param: id - timer to remove from registry.
	// @Return: void.
	// --------------------------------------------------------
	void unregisterTimer(TimerId_t id);

	// --------------------------------------------------------
	// @Brief: unregisters a pin observer by ID.
	// @Param: id - observer to remove from registry.
	// @Return: void.
	// --------------------------------------------------------
	void unregisterObserver(ObserverId_t id);

	// --------------------------------------------------------
	// @Brief: unregisters a message handler from message
	// server.
	// @param: msgHandler - pointer to msg handler to remove.
	// @Return: void.
	// --------------------------------------------------------
	void unregisterMessageHandler(BaseMsgHandler* msgHandler);

private:
	std::map<TimerId_t, BaseTimer*> mTimers;
	std::map<ObserverId_t, BaseObserver*> mPinWatchers;
	MsgHandler<BaseApp, messages::FaultMsg> mFaultMsgHandler;
	uint64_t mLastTimestamp = 0U;

	// --------------------------------------------------------
	// @Brief: goes through all timers and updates the 
	// current time. If timer has elapsed, the callback will
	// be triggered.
	// @Return: void.
	// --------------------------------------------------------
	void updateTimers(void);

	// --------------------------------------------------------
	// @Brief: checks each watched pin and triggers the
	// callback if the state has changed.
	// @Return: void.
	// --------------------------------------------------------
	void checkWatchedPins(void);

	// --------------------------------------------------------
	// @Brief: message callback when Fault msg is generated.
	// @param: msg - pointer to the fault message.
	// @Return: void.
	// --------------------------------------------------------
	void handleCriticalFaultMsg(const messages::FaultMsg* msg);
};

}
}

#endif