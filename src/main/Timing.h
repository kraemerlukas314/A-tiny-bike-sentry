/**
 * @file Timing.h
 *
 * Header file for Timing class
 *
 * @author Lukas Krämer
 */
#pragma once

#include "Arduino.h"

class Timing {
  public:
    Timing(byte clock_freq_mhz);
    void delay(int d);
    long long millis();
  private:
    byte this_clock_freq_mhz;
};
