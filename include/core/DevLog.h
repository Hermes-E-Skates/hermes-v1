// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef DEVLOG_H_
#define DEVLOG_H_


#include <Arduino.h>
#include <cstring>
#include "CommonDefs.h"


namespace hermes {

#define __FILENAME__ (std::strrchr(__FILE__, '\\') ? std::strrchr(__FILE__, '\\') + 1 : __FILE__)
#define DLOG_DEBUG(fmt, ...) if( DevLog::level >= DEBUG )       { DevLog::print(("%s: %d: %s(): " fmt "\r\n"), __FILENAME__, __LINE__, __func__, ##__VA_ARGS__); } 
#define DLOG_INFO(fmt, ...) if( DevLog::level >= INFO )       { DevLog::print(("%s: %d: %s(): " fmt "\r\n"), __FILENAME__, __LINE__, __func__, ##__VA_ARGS__); } 
#define DLOG_WARNING(fmt, ...) if( DevLog::level >= WARNING )   { DevLog::print(("%s: %d: %s(): " fmt "\r\n"), __FILENAME__, __LINE__, __func__, ##__VA_ARGS__); } 
#define DLOG_ERROR(fmt, ...) if( DevLog::level >= ERROR )       { DevLog::print(("%s: %d: %s(): " fmt "\r\n"), __FILENAME__, __LINE__, __func__, ##__VA_ARGS__); } 
#define DLOG_CRITICAL(fmt, ...) if( DevLog::level >= CRITICAL ) { DevLog::print(("%s: %d: %s(): " fmt "\r\n"), __FILENAME__, __LINE__, __func__, ##__VA_ARGS__); } 


class DevLog
{
public:
    // ---------------------------------------------------------------
    // @Brief: initialized the serial port.
    // @Param: userData - unused.
    // @Return: bool - returns true if function executed ok.
    // ---------------------------------------------------------------
    static void init();

    // ---------------------------------------------------------------
    // @Brief: function to implement printf %d, %u, %f etc style of
    //   printing.
    // @Param: msg - message and format. 
    // @Param: ... - variable arguments. 
    // @Return: int - returns the status.
    // ---------------------------------------------------------------
    static int print(const char* msg, ...);

    static DevLogLevel_t level;

private:
    // ---------------------------------------------------------------
    // @Brief: Constructor is empty.
    // @Return: void
    // ---------------------------------------------------------------
    DevLog();
};
}
#endif