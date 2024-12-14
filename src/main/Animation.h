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

/**
 * Animation class that provides
 * functions for various animations (e.g. when switching states)
 *
 */
class Animation {
   public:
    Animation();
    void enter_sentry();
    void enter_deep_sleep();
    void in_sentry();
    void in_attention();
    void exit_alarm();
    void in_alarm();

   private:
    Timing timing;
    Gpio gpio;
    bool led_state;
};
