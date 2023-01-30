// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/core/MessageServer.h"


namespace eskates {
namespace core {

MessageServer* MessageServer::mInstance = nullptr;

MessageServer::MessageServer()
{
	return;
}

MessageServer* MessageServer::getInstance()
{
	if (mInstance == nullptr) {
		mInstance = new MessageServer();
	}
	return mInstance;
}

bool MessageServer::publishMessage(const messages::Message* msg) const
{
	bool found = false;

	auto it = mMsgHandlers.begin();
	while (it != mMsgHandlers.end()) {
		if ((*it)->getMsgId() == msg->getId()) {
			(*it)->executeCallback(msg);
			found = true;
		}
		it++;
	}

	return found;
}

void MessageServer::registerListener(BaseMsgHandler* handler)
{
	mMsgHandlers.push_back(handler);
}

bool MessageServer::unregisterListener(BaseMsgHandler* handler)
{
	bool found = false;

	auto it = mMsgHandlers.begin();
	while (it != mMsgHandlers.end()) {
		if ((*it) == handler) {
			mMsgHandlers.erase(it);
			found = true;
			break;
		}
		else {
			it++;
		}
	}

	return found;
}

}
}