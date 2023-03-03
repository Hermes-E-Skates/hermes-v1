// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/messages/ChargeRdyMsg.h"


namespace hermes {
namespace messages {

ChargeRdyMsg::ChargeRdyMsg(bool rdyToCharge)
	: Message(CHARGE_RDY_MSG)
	, mRdyToCharge(rdyToCharge)
{
	return;
}

bool ChargeRdyMsg::isReadyToCharge(void) const
{
	return mRdyToCharge;
}

}
}