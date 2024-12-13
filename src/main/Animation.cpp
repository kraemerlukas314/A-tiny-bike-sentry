/**
 * @file Animation.cpp
 *
 * Cpp file for Animation class that provides 
 * functions for hardware animations (e.g. LEDS, Buzzers)
 *
 * @author Lukas Krämer
 */
#pragma once

#include "Arduino.h"

#include "Animation.h"
#include "Timing.h"
#include "defines.h"

Animation::Animation() {
  pinMode(PIN_BUTTON, INPUT);
  Timing timing(CLOCK_FREQ_MHZ);
}

void Animation::toggle(byte pin, int d) {
  digitalWrite(pin, HIGH);
  timing.delay(d);
  digitalWrite(pin, LOW);
  timing.delay(d);
}
