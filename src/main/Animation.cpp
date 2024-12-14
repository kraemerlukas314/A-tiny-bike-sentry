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

/**
 * Constructor for Animation class.
 *
 * @param clock_freq_mhz selected clock frequency.
 */
Animation::Animation() : timing(CLOCK_FREQ_MHZ), gpio() {
    gpio.setup();
    led_state = false;
}

/**
 * Animation to play when enterying sentry state.
 *
 */
void Animation::enter_sentry() {
    gpio.toggle(PIN_LED, 3, 100);
}

/**
 * Animation to play when enterying deep sleep state.
 *
 */
void Animation::enter_deep_sleep() {
    gpio.toggle(PIN_LED, 2, 400);
}

/**
 * Animation to play when in sentry mode.
 *
 */
void Animation::in_sentry() {
    gpio.off(PIN_LED);
}

/**
 * Animation to play when in attention mode.
 *
 */
void Animation::in_attention() {
    gpio.on(PIN_LED);
}

/**
 * Animation to play when exiting alarm mode.
 *
 */
void Animation::exit_alarm() {
    gpio.off(PIN_LED);
    gpio.off(PIN_BUZZER);
}

/**
 * Animation to play when in alarm mode.
 *
 */
void Animation::in_alarm() {
    gpio.set_pin(PIN_LED, led_state);
    gpio.set_pin(PIN_BUZZER, !led_state);
    led_state = !led_state;
}