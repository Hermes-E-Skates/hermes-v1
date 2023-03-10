// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#ifndef COMMONDEFS_H_
#define COMMONDEFS_H_


#include <stdint.h>
#include <Arduino.h>


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
    CHARGE_STATUS_MSG,
    CHARGE_RDY_MSG,
    BLUETOOTH_STATUS_MSG,
    SLEEP_STATE_MSG
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

enum MaxSpeed_t
{
    KMH5 = 0,
    KMH10 = 1,
    KMH15 = 2,
    KMH20 = 3,
    KMH25 = 4,
    KMH30 = 5,
    KMH35 = 6,
    UNLIMITED = 7
};

enum MaxAccel_t
{
    SLOW = 0,
    NORMAL = 1,
    FAST = 2,
    RACE = 3
};

enum Control_t
{
    LOAD_SENSOR = 0,
    BLUETOOTH = 1,
    MOTION = 2,
    OFF = 3
};

enum Pin_t : int8_t
{
    RX_PIN = 0,
    TX_PIN = 1,
    ESC_PWM_PIN = 46,
    B1_PIN = 9,
    GPIO_1_PIN = 44, // identifier
    GPIO_2_PIN = 43, // load 1 sck
    RX_BT_PIN = 14,
    TX_BT_PIN = 15,
    BAT_ALERT_PIN = 5,
    SCL_PIN = 21,
    SDA_PIN = 20,
    USBC_FLIP_PIN = 33,
    USBC_FAULT_PIN = 34,
    I2C_EN_PIN = 30,
    SCK_PIN = 52, // load 2 sck
    MOSI_PIN = 51, // load 1 dt
    MISO_PIN = 50, // load 2 dt
    CHG_LED = A1,
    MOT_EN_LED = A3,
    USER_LED = A4,
    MOTOR_SENSE_H1 = A14,
    MOTOR_SENSE_H2 = A13,
    MOTOR_SENSE_H3 = A12,
    MOTOR_SENSE_TPM = A15,
    MOT_EN = A0
};

enum DevLogLevel_t : int8_t
{
    NONE = 0,
    CRITICAL = 1,
    ERROR = 2,
    WARNING = 3,
    INFO = 4,
    DEBUG = 5
};

enum Identifier_t : int8_t
{
    UNKNOWN_ID = 0,
    LEFT_SKATE = 0x5A,
    RIGHT_SKATE = 0x5B,
};

enum TimerType_t
{
    ONESHOT,
    PERIODIC
};

}

#endif