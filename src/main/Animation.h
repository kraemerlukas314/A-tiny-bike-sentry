/**
 * @file Animation.h
 *
 * Header file for Animation class
 *
 * @author Lukas Krämer
 */
#pragma once

#include "Arduino.h"
#include "Timing.h"
#include "defines.h"

class Animation {
  public:
    Animation();
    void on(byte pin);
    void off(byte pin);
    void set_pin(byte pin, bool state);
    void toggle(byte pin, int iterations, long d);
  private:
    Timing timing;
};
