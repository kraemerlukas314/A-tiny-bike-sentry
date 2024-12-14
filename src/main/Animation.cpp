/**
 * @file Animation.cpp
 *
 * Cpp file for Animation class that provides
 * functions for various animations (e.g. when switching states)
 *
 * @author Lukas Krämer
 */
#pragma once

#include "Animation.h"

#include "Arduino.h"
#include "Gpio.h"
#include "Timing.h"
#include "defines.h"

Animation::Animation() : timing(CLOCK_FREQ_MHZ), gpio() {
    gpio.setup();
    led_state = false;
}

void Animation::enter_sentry() {
    gpio.toggle(PIN_LED, 3, 100);
}

void Animation::enter_deep_sleep() {
    gpio.toggle(PIN_LED, 2, 400);
}

void Animation::in_sentry() {
    gpio.off(PIN_LED);
}

void Animation::in_attention() {
    gpio.on(PIN_LED);
}

void Animation::exit_alarm() {
    gpio.off(PIN_LED);
    gpio.off(PIN_BUZZER);
}

void Animation::alarm() {
    gpio.set_pin(PIN_LED, led_state);
    gpio.set_pin(PIN_BUZZER, !led_state);
    led_state = !led_state;
}