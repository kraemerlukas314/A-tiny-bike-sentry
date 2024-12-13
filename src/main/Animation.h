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
    void toggle(byte pin, int d);
    void on(byte pin);
    void off(byte pin);
  private:
    Timing timing;
};
