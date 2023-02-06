// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef CRITICALFAULT_H_
#define CRITICALFAULT_H_


#include <string>
#include "CommonDefs.h"


namespace hermes {
namespace core {

enum FaultCode_t : int8_t {
	UNKNOWN_FAULT = -0x01,
	BATTERY_CRITICAL = 0x01,
	BATTERY_OVERTEMP = 0x02,
	BATTERY_OVERCURRENT = 0x03,
	BATTERY_OVERVOLTAGE = 0x04,
	BATTERY_SHORTCIRCUIT = 0x05,
	BATTERY_UNDERVOLT = 0x06,
	BATTERY_CHIP_ERR = 0x06,
	CHARGER_SHORTCIRCUIT = 0x10,
	CHARGER_BATTERY_ERR = 0x11,
	CHARGER_INPUTOVERVOLTAGE = 0x12,
	CHARGER_INPUTOVERCURRENT = 0x13,
	IMU_GENERAL_ERR = 0x20
};

enum FaultSrc_t {
	UNKNOWN_SRC,
	BATTERY,
	MOTOR,
	CHARGER,
	BLUETOOTH
};

class CriticalFault {
public:
	CriticalFault(FaultCode_t code, FaultSrc_t src, uint64_t time, std::string const& message);
	~CriticalFault();
	FaultCode_t getFaultCode(void) const;
	FaultSrc_t getSource(void) const;
	uint64_t getTimeOfOccurence(void) const;
	std::string getMessage(void) const;

private:
	FaultCode_t mFaultCode = UNKNOWN_FAULT;
	FaultSrc_t mFaultSrc = UNKNOWN_SRC;
	uint64_t mTimeOfOccurence = 0;
	std::string mMessage;
};
}
}

#endif // CRITICALFAULT_H_