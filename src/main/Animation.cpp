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
#include "defines.h"
#include "Timing.h"

Animation::Animation() : timing(CLOCK_FREQ_MHZ) {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
}

void Animation::on(byte pin) {
  digitalWrite(pin, HIGH);
}

void Animation::off(byte pin) {
  digitalWrite(pin, LOW);
}

void Animation::set_pin(byte pin, bool state) {
  digitalWrite(pin, state);
}

/**
 * Function to toggle a digital pin on and off.
 * 
 * @param pin Pin to toggle on and off
 * @param iterations How often should pin be toggled
 * @param t delay between of/off states (t = 1 / frequency)
 */
void Animation::toggle(byte pin, int iterations, long d) {
  for (int i = 0; i < iterations; ++i) {
    digitalWrite(pin, HIGH);
    timing.wait_ms(d);
    digitalWrite(pin, LOW);
    timing.wait_ms(d);
  }
}
