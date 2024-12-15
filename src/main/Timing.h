/**
 * @file Timing.h
 *
 * Header file for Timing class.
 *
 * @author Lukas Krämer
 */
#pragma once

#include "Arduino.h"

/**
 * Timing class that provides corrected
 * delay and millis functions based on selected clock frequency.
 *
 */
class Timing {
   public:
    Timing(byte clock_freq_mhz);
    void wait_ms(long d);
    long long get_millis();

   private:
    byte this_clock_freq_mhz;  ///< selected clock frequency
};
