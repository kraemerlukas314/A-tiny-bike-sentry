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
  private:
    int test;
    Timing timing(CLOCK_FREQ_MHZ);
};
