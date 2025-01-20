/**
 * @file defines.h
 *
 * This file includes all important declarations and definitions.
 *
 * @author Lukas Krämer
 */

/**
 * @brief Pin for the LED.
 */
#define PIN_LED 1

/**
 * @brief Pin for the button.
 */
#define PIN_BUTTON 2

/**
 * @brief Pin for the piezo.
 */
#define PIN_PIEZO 3

/**
 * @brief Pin for the button.
 */
#define PIN_BUZZER 4

/**
 * @brief Selected clock frequency in MHz.
 */
#define CLOCK_FREQ_MHZ 1

/**
 * @brief Threshold for piezo for movement detection.
 */
#define THRESHOLD_PIEZO 5

/**
 * @brief Button debounce delay.
 */
#define DELAY_BUTTON_DEBOUNCE_MS 50

/**
 * @brief Frequency for alarm.
 */
#define ALARM_TOGGLE_FREQ 20

/**
 * @brief When piezo is moved, wait
 * for DELAY_PIEZO_MOVED_MS because it could be just a button press.
 */
#define DELAY_PIEZO_MOVED_MS 500

/**
 * @brief Cooldown after which state attention is left again.
 */
#define ATTENTION_COOLDOWN_MS 10000

/**
 * @brief Cooldown after which state alarm is left again.
 */
#define ALARM_COOLDOWN_MS 10000
