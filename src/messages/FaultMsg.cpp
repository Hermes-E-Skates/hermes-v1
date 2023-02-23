// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/messages/FaultMsg.h"


namespace hermes {
namespace messages {

FaultMsg::FaultMsg(core::CriticalFault fault)
	: Message(CRITICAL_FAULT_MSG)
	, mCriticalFault(fault)
{
	return;
}

core::CriticalFault const& FaultMsg::getFaultObject(void) const
{
	return mCriticalFault;
}

}
}