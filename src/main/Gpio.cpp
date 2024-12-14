/**
 * @file Gpio.cpp
 *
 * Cpp file for Gpio class that provides
 * functions for direct hardware actions (e.g. turning on/off digital pins)
 *
 * @author Lukas Krämer
 */
#pragma once

#include "Gpio.h"

#include "Arduino.h"
#include "Timing.h"
#include "defines.h"

Gpio::Gpio() : timing(CLOCK_FREQ_MHZ) {
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
}

void Gpio::setup() {
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
}

void Gpio::on(byte pin) {
    digitalWrite(pin, HIGH);
}

void Gpio::off(byte pin) {
    digitalWrite(pin, LOW);
}

void Gpio::set_pin(byte pin, bool state) {
    digitalWrite(pin, state);
}

/**
 * Function to toggle a digital pin on and off.
 *
 * @param pin Pin to toggle on and off
 * @param iterations How often should pin be toggled
 * @param t delay between of/off states (t = 1 / frequency)
 */
void Gpio::toggle(byte pin, int iterations, long d) {
    for (int i = 0; i < iterations; ++i) {
        digitalWrite(pin, HIGH);
        timing.wait_ms(d);
        digitalWrite(pin, LOW);
        timing.wait_ms(d);
    }
}
