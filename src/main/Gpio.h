/**
 * @file Gpio.h
 *
 * Header file for Gpio class
 *
 * @author Lukas Krämer
 */
#pragma once

#include "Arduino.h"
#include "Timing.h"
#include "defines.h"

class Gpio {
   public:
    Gpio();
    void setup();
    void on(byte pin);
    void off(byte pin);
    void set_pin(byte pin, bool state);
    void toggle(byte pin, int iterations, long d);

   private:
    Timing timing;
};
