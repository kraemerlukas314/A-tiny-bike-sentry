#include <avr/sleep.h>
#include <avr/interrupt.h>

#define PIN_LED 1
#define PIN_BUTTON 2
#define PIN_PIEZO 3
#define PIN_BUZZER 4

#define THRESHOLD_PIEZO 30
#define DELAY_BUTTON_DEBOUNCE_MS 50
#define ALARM_TOGGLE_FREQ 20

// after piezo detects movement, wait for DELAY_PIEZO_MOVED_MS and check if it is actually a button press
#define DELAY_PIEZO_MOVED_MS 500


// TODO:
/**
 * Interrupt
 * Deep sleep
 * Pulsierende Animation im Sentry Mode
 * Watch Dog
 */
enum State {
  DEEP_SLEEP,
  SENTRY,
  ATTENTION,
  ALARM
};

long long last_led_toggle = 0;
bool led_state = true;

State state = DEEP_SLEEP;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_PIEZO, INPUT);
  pinMode(PIN_BUTTON, INPUT);
}

void loop() {
  switch (state) {
    case DEEP_SLEEP:
      if (button_pressed()) {
        toggle(PIN_LED, 5, 50);
        state = SENTRY;
      }
      break;
    case SENTRY:
      analogWrite(PIN_LED, 10);
      if (button_pressed()) {
        digitalWrite(PIN_LED, LOW);
        state = DEEP_SLEEP;
      } else if (piezo_moved()) {
        state = ATTENTION;
      }
      break;
    case ATTENTION:
      digitalWrite(PIN_LED, HIGH);
      if (button_pressed()) {
        toggle(PIN_LED, 2, 300);
        state = DEEP_SLEEP;
      } else if (piezo_moved()) {
        state = ALARM;
      }
      break;
    case ALARM:
      if (button_pressed()) {
        toggle(PIN_LED, 2, 400);
        state = DEEP_SLEEP;
      } else if (millis() - last_led_toggle > (1000 / ALARM_TOGGLE_FREQ)) {
        last_led_toggle = millis();
        led_state = !led_state;
        digitalWrite(PIN_LED, led_state);
      }
      break;
  }
}

bool button_pressed() {
  if (!digitalRead(PIN_BUTTON)) return false;
  while (digitalRead(PIN_BUTTON));
  delay(10);
  return true;
}

bool piezo_moved() {
  if (analogRead(PIN_PIEZO) <= THRESHOLD_PIEZO) return false;

  // wait for potential button press
  long long start_time = millis();
  while (millis() - start_time < 200) {
    if (digitalRead(PIN_BUTTON)) {
      while (digitalRead(PIN_BUTTON));
      delay(50);
      return false;
    }
  }
  return true;
}

void toggle(byte pin, int iterations, int t) {
  for (int i = 0; i < iterations; ++i) {
    digitalWrite(pin, HIGH);
    delay(t);
    digitalWrite(pin, LOW);
    delay(t);
  }
}
