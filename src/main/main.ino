#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#define PIN_LED 1
#define PIN_BUTTON 2
#define PIN_PIEZO 3
#define PIN_BUZZER 4

#define THRESHOLD_PIEZO 30
#define DELAY_BUTTON_DEBOUNCE_MS 50
#define ALARM_TOGGLE_FREQ 20

#define DELAY_PIEZO_MOVED_MS 500
#define ATTENTION_COOLDOWN_MS 5000
#define ALARM_COOLDOWN_MS 5000

#define CLOCK_FREQ_MHZ 1

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

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_PIEZO, INPUT);
  pinMode(PIN_BUTTON, INPUT);
  while (1) {
    digitalWrite(PIN_LED, HIGH);
    _delay(500);
    digitalWrite(PIN_LED, LOW);
    _delay(500);
  }

  // Configure the button pin for pin change interrupt
  configure_button_interrupt();

  // Configure the Watchdog Timer
  configure_watchdog();
}

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
        timestamp_entered_attention = _millis();
        state = ATTENTION;
      }
      break;
    case ATTENTION:
      digitalWrite(PIN_LED, HIGH);
      if (_millis() - timestamp_entered_attention > ATTENTION_COOLDOWN_MS) {
        state = SENTRY;
      }
      if (button_pressed()) {
        toggle(PIN_LED, 2, 400);
        state = DEEP_SLEEP;
      } else if (piezo_moved()) {
        timestamp_entered_alarm = _millis();
        state = ALARM;
      }
      break;
    case ALARM:
      if (_millis() - timestamp_entered_alarm > ALARM_COOLDOWN_MS) {
        timestamp_entered_attention = _millis();
        state = ATTENTION;
      }
      if (button_pressed()) {
        toggle(PIN_LED, 2, 400);
        state = DEEP_SLEEP;
      } else if (_millis() - timestamp_last_led_toggle > (1000 / ALARM_TOGGLE_FREQ)) {
        timestamp_last_led_toggle = _millis();
        led_state = !led_state;
        digitalWrite(PIN_LED, led_state);
      }
      break;
  }
}

void configure_button_interrupt() {
  cli();  // Disable interrupts
  GIMSK |= _BV(PCIE);  // Enable Pin Change Interrupts
  PCMSK |= _BV(PCINT2);  // Enable interrupt on PIN_BUTTON (PB2)
  sei();  // Enable interrupts
}

ISR(PCINT0_vect) {
  // Handle button press interrupt
  if (!digitalRead(PIN_BUTTON)) {
    state = DEEP_SLEEP;  // Enter sleep state on button press
  }
}

void configure_watchdog() {
  cli();  // Disable interrupts
  wdt_reset();
  WDTCR = (1 << WDP3) | (1 << WDP0); // Set WDT timeout to 5 seconds
  WDTCR |= (1 << WDE) | (1 << WDIE); // Enable WDT and WDT interrupt
  sei();  // Enable interrupts
}

void enable_watchdog() {
  cli();
  wdt_reset();
  WDTCR = (1 << WDP3) | (1 << WDP0); // Set WDT timeout to 5 seconds
  WDTCR |= (1 << WDE) | (1 << WDIE); // Enable WDT and WDT interrupt
  sei();
}

void disable_watchdog() {
  cli();
  wdt_reset();
  WDTCR &= ~(1 << WDE); // Disable the WDT system reset
  sei();
}

ISR(WDT_vect) {
  // Watchdog timer interrupt: Enter sleep mode
  state = DEEP_SLEEP;
  // Re-enable button interrupt in case it was disabled
  configure_button_interrupt();
}

bool button_pressed() {
  if (!digitalRead(PIN_BUTTON)) return false;
  while (digitalRead(PIN_BUTTON));
  _delay(DELAY_BUTTON_DEBOUNCE_MS);
  return true;
}

bool piezo_moved() {
  if (analogRead(PIN_PIEZO) <= THRESHOLD_PIEZO) return false;

  long start_time = _millis();
  while (_millis() - start_time < DELAY_PIEZO_MOVED_MS) {
    if (digitalRead(PIN_BUTTON)) {
      while (digitalRead(PIN_BUTTON));
      _delay(DELAY_BUTTON_DEBOUNCE_MS);
      return false;
    }
  }
  return true;
}

void toggle(byte pin, int iterations, int t) {
  for (int i = 0; i < iterations; ++i) {
    digitalWrite(pin, HIGH);
    _delay(t);
    digitalWrite(pin, LOW);
    _delay(t);
  }
}

void sleep() {
  while (digitalRead(PIN_BUTTON));
  _delay(DELAY_BUTTON_DEBOUNCE_MS);
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
  _delay(DELAY_BUTTON_DEBOUNCE_MS);
}

void _delay(long long t) {
  if (CLOCK_FREQ_MHZ == 1) delay(t * 8);
  else if (CLOCK_FREQ_MHZ == 8) delay(t);
}

long long _millis() {
  if (CLOCK_FREQ_MHZ == 1) return millis() / 8;
  if (CLOCK_FREQ_MHZ == 8) return millis();
}
