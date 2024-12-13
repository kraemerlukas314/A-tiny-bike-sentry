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

void Animation::toggle(byte pin, int d) {
  digitalWrite(pin, HIGH);
  timing.wait_ms(d);
  digitalWrite(pin, LOW);
  timing.wait_ms(d);
}
