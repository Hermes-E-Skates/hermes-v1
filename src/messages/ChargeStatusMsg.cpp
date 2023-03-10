// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/messages/ChargeStatusMsg.h"


namespace hermes {
namespace messages {

ChargeStatusMsg::ChargeStatusMsg(uint16_t chargePercent)
	: Message(CHARGE_STATUS_MSG)
	, mChargePercentage(chargePercent)
{
	return;
}

uint16_t ChargeStatusMsg::getChargePercent(void) const
{
	return mChargePercentage;
}

}
}