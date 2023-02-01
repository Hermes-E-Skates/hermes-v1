// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef MESSAGESERVER_H_
#define MESSAGESERVER_H_


#include <queue>
#include <vector>
#include "CommonDefs.h"
#include "../messages/Message.h"
#include "MessageHandler.h"


namespace hermes {
namespace core {

class MessageServer {
public:
	static MessageServer* getInstance();
	MessageServer(void);
	MessageServer(MessageServer&&) = delete;
	MessageServer& operator=(const MessageServer&) = delete;
	MessageServer& operator=(MessageServer&&) = delete;

	bool publishMessage(const messages::Message* msg) const;
	void registerListener(BaseMsgHandler* handler);
	bool unregisterListener(BaseMsgHandler* handler);

private:
	static MessageServer* mInstance;
	std::vector<BaseMsgHandler*> mMsgHandlers;
};
}
}

#endif // MESSAGESERVER_H_