// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include "../../include/core/DevLog.h"


namespace hermes {

DevLogLevel_t DevLog::level = DEBUG;

// ---------------------------------------------------------------
// @Brief: Constructor is empty.
// @Return: void
// ---------------------------------------------------------------
DevLog::DevLog()
{
    return;
}

// ---------------------------------------------------------------
// @Brief: initialized the Serial3 port.
// @Param: userData - unused.
// @Return: bool - returns true if function executed ok.
// ---------------------------------------------------------------
void DevLog::init()
{
    level = DEBUG;
    Serial3.begin(BAUD_115200);
}

// ---------------------------------------------------------------
// @Brief: function to implement printf %d, %u, %f etc style of
//   printing.
// @Param: msg - message and format. 
// @Param: ... - variable arguments. 
// @Return: int - returns the status.
// ---------------------------------------------------------------
int DevLog::print(const char* fmt, ...)
{
    char buff[256];
    va_list args;
    va_start(args, fmt);
    int return_status = vsnprintf(buff, sizeof(buff), fmt, args);
    va_end(args);
    Serial3.print(buff);
    return return_status;
}

}