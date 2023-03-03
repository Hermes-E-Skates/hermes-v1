// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef BLUETOOTHSTATUSMSG_H_
#define BLUETOOTHSTATUSMSG_H_


#include "Message.h"


namespace hermes {
namespace messages {

class BluetoothStatusMsg : public Message
{
public:
	BluetoothStatusMsg(bool isConnected);
	bool isConnected(void) const;

private:
	bool mIsConnected;
};

}
}
#endif // BLUETOOTHSTATUSMSG_H_