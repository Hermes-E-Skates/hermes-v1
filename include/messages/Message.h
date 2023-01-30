// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef MESSAGE_H_
#define MESSAGE_H_


#include "../core/CommonDefs.h"


namespace eskates {
namespace messages {

class Message {
public:
	Message(MsgId_t id);
	virtual ~Message();
	MsgId_t getId() const;

private:
	MsgId_t mMsgId;
};
}
}

#endif // MESSAGE_H_