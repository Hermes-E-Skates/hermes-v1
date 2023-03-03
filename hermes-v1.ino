// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of the Hermes Eskate project group and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------



#include <Wire.h>
#include <ArduinoSTL.h>
#include "include/core/EventLoop.h"
#include "include/HermesController.h"
#include "include/messages/SleepStateMsg.h"

using namespace hermes;

HermesController controller;

void setup()
{
    // Lag the bootup to ensure all IC's are at a stable voltage
    delay(1000);
    core::EventLoop::getInstance()->init();
    controller.changeState(HermesController::MOTOR_DISABLED);
  }

void loop()
{
    core::EventLoop::getInstance()->processEvents();
}
