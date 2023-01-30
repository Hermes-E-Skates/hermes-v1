#include "../../include/cmds/BluetoothResponse.h"

namespace eskates {
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
	encodeData(mEncodedResponse);
	mEncodedResponse.push_back(mAck ? ACK : NAK);
	mEncodedResponse.push_back(EOT);
	return &mEncodedResponse[0];
}

CmdId_t BluetoothResponse::getCmdId(void) const
{
	return mCmdId;
}

}
}