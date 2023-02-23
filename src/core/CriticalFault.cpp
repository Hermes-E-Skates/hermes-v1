// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/core/CriticalFault.h"


namespace hermes {
namespace core {

CriticalFault::CriticalFault(FaultCode_t code, FaultSrc_t source, uint64_t time, std::string const& message)
	: mFaultCode(code)
	, mFaultSrc(source)
	, mTimeOfOccurence(time)
	, mMessage(message)
{
	return;
}

CriticalFault::~CriticalFault(void)
{
	return;
}

FaultCode_t CriticalFault::getFaultCode(void) const
{
	return mFaultCode;
}

FaultSrc_t CriticalFault::getSource(void) const
{
	return mFaultSrc;
}

uint64_t CriticalFault::getTimeOfOccurence(void) const
{
	return mTimeOfOccurence;
}

std::string CriticalFault::getMessage(void) const
{
	return mMessage;
}

}
}