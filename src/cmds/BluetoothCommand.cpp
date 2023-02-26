// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/cmds/BluetoothCommand.h"


namespace hermes {
namespace bt {


BluetoothCommand::BluetoothCommand(CmdId_t id)
	: mCmdId(id)
	, mValid(true)
{
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

void BluetoothCommand::setValid(bool valid)
{
	mValid = valid;
}

}
}