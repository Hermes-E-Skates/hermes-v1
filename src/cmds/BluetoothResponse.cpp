// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/cmds/BluetoothResponse.h"


namespace hermes {
namespace bt {

BluetoothResponse::BluetoothResponse(CmdId_t id, bool ack)
	: mCmdId(id)
	, mAck(ack)
{
	return;
}

uint8_t BluetoothResponse::getSize(void) const
{
	return static_cast<uint8_t>(mEncodedResponse.size());
}

const uint8_t* BluetoothResponse::encode(void)
{
	mEncodedResponse.push_back(STX);
	mEncodedResponse.push_back(mCmdId);
	mEncodedResponse.push_back(mAck ? ACK : NAK);
	mEncodedResponse.push_back(0); // Placeholder
	encodeData(mEncodedResponse);
	mEncodedResponse[3] = mEncodedResponse.size() - 4;
	return mEncodedResponse.data();
}

CmdId_t BluetoothResponse::getCmdId(void) const
{
	return mCmdId;
}

}
}