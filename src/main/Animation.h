/**
 * @file Animation.h
 *
 * Header file for Animation class
 *
 * @author Lukas Krämer
 */
#pragma once

#include "Arduino.h"
#include "Gpio.h"
#include "Timing.h"
#include "defines.h"

class Animation {
   public:
    Animation();
    void enter_sentry();
    void enter_deep_sleep();
    void in_sentry();
    void in_attention();
    void exit_alarm();
    void alarm();

   private:
    Timing timing;
    Gpio gpio;
    bool led_state;
};
