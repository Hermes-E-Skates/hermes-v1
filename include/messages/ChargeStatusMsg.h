// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef CHARGESTATUSMSG_H_
#define CHARGESTATUSMSG_H_


#include "Message.h"


namespace eskates {
namespace messages {

class ChargeStatusMsg : public Message
{
public:
	ChargeStatusMsg(uint16_t chargePercent);
	uint16_t getChargePercent(void) const;

private:
	uint16_t mChargePercentage;
};
}
}
#endif // CHARGESTATUSMSG_H_