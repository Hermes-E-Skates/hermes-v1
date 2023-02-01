// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/hw/BluetoothInterface.h"
#include "../../include/core/DevLog.h"


namespace hermes {
namespace bt {

uint8_t inline findByte(uint8_t target, uint8_t* const bytes, uint8_t len)
{
	for (uint8_t i = 0; i < len; i++) {
		if (bytes[i] = target) return i;
	}
	return NOT_FOUND;
}

BluetoothInterface::BluetoothInterface()
	: BaseApp()
	, mHeartbeatHandler(this, &BluetoothInterface::handleHeartbeat, HEARTBEAT)
	, mHeartbeatTimer(this, &BluetoothInterface::onHeartbeatTimerExpire)
{
	return;
}

bool BluetoothInterface::init()
{
	Serial3.begin(BAUD_9600);
	return true;
}

void BluetoothInterface::loop()
{
	int16_t bytesAvail = Serial3.available();

	if (bytesAvail) {
		uint8_t* buffer = new uint8_t[bytesAvail];
		int16_t bytesRead = Serial3.readBytes(buffer, bytesAvail);

		if (bytesRead == bytesAvail) {
			const BluetoothCommand* cmd = parseCommand(buffer, bytesRead);
			if (cmd != nullptr) {
				handleNewMessage(cmd);
				delete cmd;
			}
		}
	}
	return;
}

void BluetoothInterface::onCriticalFault(const core::CriticalFault& criticalFault)
{
	return;
}

const BluetoothCommand* BluetoothInterface::parseCommand(uint8_t* const buffer, uint8_t len)
{
	BluetoothCommand* cmd = nullptr;

	do {
		uint8_t stxBytePos = bt::findByte(STX, buffer, len);
		uint8_t eotBytePos = bt::findByte(EOT, buffer, len);

		if (stxBytePos == NOT_FOUND || eotBytePos == NOT_FOUND) {
			// We NEED both the STX and EOT bytes
			break;
		}

		uint8_t cmdLen = eotBytePos - stxBytePos + 1;
		if (cmdLen < 3) {
			// We NEED at least 3 bytes, STX ID EOT
			break;
		}

		uint8_t cmdIdByte = buffer[stxBytePos + 1];
		if (cmdIdByte <= UNKNOWN_CMD || cmdIdByte >= CMD_ID_END) {
			// CMD ID must be valid
			break;
		}

		bool dataLen = cmdLen - 3;
		const uint8_t* data = nullptr;

		if (dataLen > 0) {
			data = buffer + stxBytePos + 2;
		}

		CmdId_t cmdId = static_cast<CmdId_t>(cmdIdByte);
		DLOG_DEBUG("New cmd with id=%d", cmdId);

		switch(cmdId) {
		case HEARTBEAT:
			cmd = new HeartbeatCmd(buffer, cmdLen);
			break;

		case GET_ID:
			cmd = new GetIdCmd(buffer, cmdLen);
			break;

		case GET_INFO:
			cmd = new GetInfoCmd(buffer, cmdLen);
			break;

		case GET_FAULT:
			cmd = new GetFaultCmd(buffer, cmdLen);
			break;

		case GET_CHARGER:
			cmd = new GetChargeStatusCmd(buffer, cmdLen);
			break;

		case GET_IMU:
		case GET_BATTERY:
		case SET_MODE:
		case SET_MAX_ACCEL:
		case SET_MAX_SPEED:
		case SET_THROTTLE:
		default:
			break;
		}
	} while(0);

	return cmd;
}

void BluetoothInterface::handleNewMessage(const BluetoothCommand* cmd)
{
	if (cmd != nullptr && cmd->valid()) {
		for (auto it : mHandlers) {

			if (it->getCmdId() == cmd->getCmdId()) {
				BluetoothResponse* resp = it->handleCommand(cmd);

				if (resp != nullptr) {
					Serial3.write(resp->encode(), resp->getSize());
					delete resp;
				} else {
					DLOG_WARNING("Response from handler is null.");
				}
			}
		}
	}
}

void BluetoothInterface::onHeartbeatTimerExpire(uint32_t userdata)
{
	mConnected = false;
	DLOG_WARNING("Bluetooth connection lost.");
}

BluetoothResponse* BluetoothInterface::handleHeartbeat(const HeartbeatCmd* cmd)
{
	mConnected = true;
	if (mHeartbeatTimer.isEnabled()) {
		mHeartbeatTimer.restart();
	} else {
		mHeartbeatTimer.start(HEARTBEAT_INTERVAL, ONESHOT, 0);
	}
}

void BluetoothInterface::registerSerialHandler(BaseBluetoothHandler* handler)
{
	mHandlers.push_back(handler);
	return;
}

bool BluetoothInterface::isConnected(void) const
{
	return mConnected;
}

bool BluetoothInterface::sendATCommand(const std::string& AT) const
{
	if (!isConnected()) {
		Serial3.write(AT.c_str(), AT.size());
		return true;
	} else {
		return false;
	}
}

}
}