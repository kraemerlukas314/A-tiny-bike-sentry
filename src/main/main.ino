/**
 * @file main.ino
 *
 * This file includes all the main logic to switch between internal states.
 *
 * @author Lukas Krämer
 */

#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "defines.h"
#include "Animation.h"
#include "Timing.h"

/**
 * Enum to store current program state.
 * 
 */
enum State {
  DEEP_SLEEP,
  SENTRY,
  ATTENTION,
  ALARM
};

long long timestamp_last_led_toggle;
long long timestamp_entered_attention;
long long timestamp_entered_alarm;
bool led_state = true;

State state = DEEP_SLEEP;
Animation animation;
Timing timing(CLOCK_FREQ_MHZ);

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_PIEZO, INPUT);
  pinMode(PIN_BUTTON, INPUT);

  // Configure the button pin for pin change interrupt
  configure_button_interrupt();

  // Configure the Watchdog Timer
  configure_watchdog();
}

// TODO: proper button debounce
// TODO: remove global variables
// TODO: variable for Interrupt port
void loop() {
  // Reset the watchdog timer at the start of each loop iteration
  wdt_reset();

  switch (state) {
    case DEEP_SLEEP:
      digitalWrite(PIN_LED, LOW);
      disable_watchdog();  // Disable WDT to save power
      sleep();
      enable_watchdog();   // Re-enable WDT after waking up
      state = SENTRY;
      toggle(PIN_LED, 3, 100);
      break;
    case SENTRY:
      digitalWrite(PIN_LED, LOW);
      if (button_pressed()) {
        toggle(PIN_LED, 2, 400);
        state = DEEP_SLEEP;
      } else if (piezo_moved()) {
        timestamp_entered_attention = timing.get_millis();
        state = ATTENTION;
      }
      break;
    case ATTENTION:
      digitalWrite(PIN_LED, HIGH);
      if (timing.get_millis() - timestamp_entered_attention > ATTENTION_COOLDOWN_MS) {
        state = SENTRY;
      }
      if (button_pressed()) {
        toggle(PIN_LED, 2, 400);
        state = DEEP_SLEEP;
      } else if (piezo_moved()) {
        timestamp_entered_alarm = timing.get_millis();
        state = ALARM;
      }
      break;
    case ALARM:
      if (timing.get_millis() - timestamp_entered_alarm > ALARM_COOLDOWN_MS) {
        timestamp_entered_attention = timing.get_millis();
        state = ATTENTION;
      }
      if (button_pressed()) {
        toggle(PIN_LED, 2, 400);
        state = DEEP_SLEEP;
      } else if (timing.get_millis() - timestamp_last_led_toggle > (1000 / ALARM_TOGGLE_FREQ)) {
        timestamp_last_led_toggle = timing.get_millis();
        led_state = !led_state;
        digitalWrite(PIN_LED, led_state);
      }
      break;
  }
}

/**
 * Configures an interrupt on the specified PIN_BUTTON.
 */
void configure_button_interrupt() {
  cli();  // Disable interrupts
  GIMSK |= _BV(PCIE);  // Enable Pin Change Interrupts
  PCMSK |= _BV(PCINT2);  // Enable interrupt on PIN_BUTTON (PB2)
  sei();  // Enable interrupts
}

/**
 * Interrupt routine for waking from sleep on button press.
 * 
 */
ISR(PCINT0_vect) {
  // Handle button press interrupt
  if (!digitalRead(PIN_BUTTON)) {
    state = DEEP_SLEEP;  // Enter sleep state on button press
  }
}

/**
 * Configures a watchdog timer with a timeout of four seconds.
 * 
 */
void configure_watchdog() {
  cli();  // Disable interrupts
  wdt_reset();
  WDTCR = (1 << WDP3) | (1 << WDP0); // Set WDT timeout to 5 seconds
  WDTCR |= (1 << WDE) | (1 << WDIE); // Enable WDT and WDT interrupt
  sei();  // Enable interrupts
}


/**
 * Enables the watchdog timer with a timeout of four seconds.
 * 
 */
void enable_watchdog() {
  cli();
  wdt_reset();
  WDTCR = (1 << WDP3) | (1 << WDP0); // Set WDT timeout to 5 seconds
  WDTCR |= (1 << WDE) | (1 << WDIE); // Enable WDT and WDT interrupt
  sei();
}

/**
 * Disables the watchdog timer.
 * 
 */
void disable_watchdog() {
  cli();
  wdt_reset();
  WDTCR &= ~(1 << WDE); // Disable the WDT system reset
  sei();
}

/**
 * Interrupt routine for the watchdog timer.
 * If watchdog triggers, enter DEEP_SLEEP state.
 * 
 */
ISR(WDT_vect) {
  // Watchdog timer interrupt: Enter sleep mode
  state = DEEP_SLEEP;
  // Re-enable button interrupt in case it was disabled
  configure_button_interrupt();
}

/**
 * Returns true/false based on current button state.
 * Also debounces the button using DELAY_BUTTON_DEBOUNCE_MS.
 * 
 * @return true button is pressed
 * @return false button isn't pressed
 */
bool button_pressed() {
  if (!digitalRead(PIN_BUTTON)) return false;
  while (digitalRead(PIN_BUTTON));
  timing.wait_ms(DELAY_BUTTON_DEBOUNCE_MS);
  return true;
}

/**
 * Returns true/false based on current piezo state.
 * 
 * @return true piezo moved
 * @return false piezo isn't moved
 */
bool piezo_moved() {
  if (analogRead(PIN_PIEZO) <= THRESHOLD_PIEZO) return false;

  long start_time = timing.get_millis();
  while (timing.get_millis() - start_time < DELAY_PIEZO_MOVED_MS) {
    if (digitalRead(PIN_BUTTON)) {
      while (digitalRead(PIN_BUTTON));
      timing.wait_ms(DELAY_BUTTON_DEBOUNCE_MS);
      return false;
    }
  }
  return true;
}

/**
 * Function to toggle a digital pin on and off.
 * 
 * @param pin Pin to toggle on and off
 * @param iterations How often should pin be toggled
 * @param t delay between of/off states (t = 1 / frequency)
 */
void toggle(byte pin, int iterations, int t) {
  for (int i = 0; i < iterations; ++i) {
    digitalWrite(pin, HIGH);
    timing.wait_ms(t);
    digitalWrite(pin, LOW);
    timing.wait_ms(t);
  }
}

/**
 * Puts ATtiny into deep sleep mode until PIN_BUTTON is pressed.
 */
void sleep() {
  while (digitalRead(PIN_BUTTON));
  timing.wait_ms(DELAY_BUTTON_DEBOUNCE_MS);
  GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
  PCMSK |= _BV(PCINT2);                   // Use PB2 as interrupt pin
  ADCSRA &= ~_BV(ADEN);                   // ADC off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // Set sleep mode

  MCUCR |= (1 << BODS) | (1 << BODSE);
  MCUCR &= ~(1 << BODSE);

  ACSR |= _BV(ACD);                       // Disable analog comparator

  sleep_enable();                         // Enable sleep mode
  sei();                                  // Enable interrupts
  sleep_cpu();                            // Go to sleep

  cli();                                  // Disable interrupts
  PCMSK &= ~_BV(PCINT2);                  // Turn off PB2 as interrupt pin
  sleep_disable();                        // Clear SE bit
  ADCSRA |= _BV(ADEN);                    // ADC on

  ACSR &= ~_BV(ACD);                      // Re-enable Analog Comparator
  sei();                                  // Enable interrupts
  while (digitalRead(PIN_BUTTON));
  timing.wait_ms(DELAY_BUTTON_DEBOUNCE_MS);
}
