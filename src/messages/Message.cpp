// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/messages/Message.h"


namespace eskates {
namespace messages {

Message::Message(MsgId_t id)
	: mMsgId(id)
{
	return;
}

Message::~Message()
{
	return;
}

MsgId_t Message::getId() const
{
	return mMsgId;
}

}
}