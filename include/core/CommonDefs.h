// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef COMMONDEFS_H_
#define COMMONDEFS_H_


#include <stdint.h>


namespace hermes {

typedef uint8_t TimerId_t;
typedef uint8_t ObserverId_t;
typedef uint8_t AppId_t;
typedef uint8_t ConditionalId_t;

constexpr uint8_t STX = 0x02;
constexpr uint8_t EOT = 0x04;
constexpr uint8_t ACK = 0x06;
constexpr uint8_t NAK = 0x15;
constexpr uint8_t NOT_FOUND = 0xFF;

typedef bool(*Conditional)();
typedef void(*Action)();

enum MsgId_t {
    UNKNOWN_MSG,
    CRITICAL_FAULT_MSG,
    CHARGE_STATUS_MSG
};

enum BaudRate_t
{
    BAUD_9600 = 9600,
    BAUD_14400 = 14400,
    BAUD_19200 = 19200,
    BAUD_28800 = 28800,
    BAUD_38400 = 38400,
    BAUD_57600 = 57600,
    BAUD_115200 = 115200
};

enum ChargeSpeed_t
{
    CURRENT_0A5 = 500 / 64,
    CURRENT_1A = 1000 / 64,
    CURRENT_3A = 3000 / 64,
    CURRENT_5A = 5000 / 64,
    MAX_SPEED = 5000 / 64
};

enum Pin_t : int8_t
{
    RX_PIN = 0,
    TX_PIN = 1,
    ESC_PWM_PIN = 46,
    B1_PIN = 51,
    GPIO_1_PIN = 44,
    GPIO_2_PIN = 43,
    RX_BT_PIN = 14,
    TX_BT_PIN = 15,
    BAT_ALERT_PIN = 5,
    SCL_PIN = 21,
    SDA_PIN = 20,
    USBC_FLIP_PIN = 33,
    USBC_FAULT_PIN = 34,
    I2C_EN_PIN = 30,
    USBC_INT_PIN = -1,
};

enum DevLogLevel_t
{
    NONE = 0,
    CRITICAL = 1,
    ERROR = 2,
    WARNING = 3,
    DEBUG = 4,
};

typedef enum TimerType_t
{
    ONESHOT,
    PERIODIC
};

}

#endif