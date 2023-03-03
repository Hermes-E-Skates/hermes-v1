// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef CHARGERDYMSG_H_
#define CHARGERDYMSG_H_


#include "Message.h"


namespace hermes {
namespace messages {

class ChargeRdyMsg : public Message
{
public:
	ChargeRdyMsg(bool rdyToCharge);
	bool isReadyToCharge(void) const;

private:
	bool mRdyToCharge;
};

}
}
#endif // SLEEPSTATEMSG_H_