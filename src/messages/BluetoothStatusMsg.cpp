// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/messages/BluetoothStatusMsg.h"


namespace hermes {
namespace messages {

BluetoothStatusMsg::BluetoothStatusMsg(bool isConnected)
	: Message(CHARGE_RDY_MSG)
	, mIsConnected(isConnected)
{
	return;
}

bool BluetoothStatusMsg::isConnected(void) const
{
	return mIsConnected;
}

}
}