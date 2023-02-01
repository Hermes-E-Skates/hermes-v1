// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef FAULTMSG_H_
#define FAULTMSG_H_


#include "Message.h"
#include "../core/CriticalFault.h"


namespace eskates {
namespace messages {

class FaultMsg : public Message
{
public:
	FaultMsg(core::CriticalFault fault);
	core::CriticalFault const& getFaultObject(void) const;

private:
	core::CriticalFault mCriticalFault;
};
}
}
#endif // FAULTMSG_H_