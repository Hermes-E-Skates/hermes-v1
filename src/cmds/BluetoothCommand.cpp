#include "../../include/cmds/BluetoothCommand.h"


namespace eskates {
namespace bt {


BluetoothCommand::BluetoothCommand(CmdId_t id, uint8_t* const data, uint8_t len)
	: mCmdId(id)
	, mValid(true)
{
	if (len > 0) {
		mValid = decodeData(data, len);
	}
	return;
}

CmdId_t BluetoothCommand::getCmdId(void) const
{
	return mCmdId;
}

bool BluetoothCommand::valid(void) const
{
	return mValid;
}

}
}