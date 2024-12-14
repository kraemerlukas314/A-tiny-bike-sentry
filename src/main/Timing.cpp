/**
 * @file Timing.cpp
 *
 * Cpp file for Timing class that provides corrected
 * delay and millis functions based on selected clock frequency.
 *
 * @author Lukas Krämer
 */
#pragma once

#include "Timing.h"

#include "Arduino.h"

/**
 * Constructor for Timing class.
 *
 * @param clock_freq_mhz selected clock frequency.
 */
Timing::Timing(byte clock_freq_mhz) {
    this_clock_freq_mhz = clock_freq_mhz;
}

/**
 * Own delay function that counteracts different clock speeds.
 *
 * @param t delay time in ms
 */
void Timing::wait_ms(long duration) {
    if (this_clock_freq_mhz == 1)
        delay(duration * 8);
    else if (this_clock_freq_mhz == 8)
        delay(duration);
    else
        delay(duration);  // Add options for external 16MHz here if needed
}

/**
 * Own millis function that counteracts different clock speeds.
 *
 * @return long millis since ATtiny restart
 */
long long Timing::get_millis() {
    if (this_clock_freq_mhz == 1) return millis() / 8;
    if (this_clock_freq_mhz == 8) return millis();
}
