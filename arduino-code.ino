// ---------------------------------------------------------------
// This software and all related information is the intellectual
// property of XXXXXXXXXXXXXXXXXXXXXXXXXXX and may not be 
// distributed, replicated or disclosed without explicit prior 
// written permission. All Rights Reserved.
// ---------------------------------------------------------------


#include <Wire.h>
#include <ArduinoSTL.h>
#include "include/core/EventLoop.h"
#include "include/EskateController.h"

using namespace eskates;

EskateController controller;

void setup()
{
    core::EventLoop::getInstance()->init();
}

void loop()
{
    core::EventLoop::getInstance()->processEvents();
}