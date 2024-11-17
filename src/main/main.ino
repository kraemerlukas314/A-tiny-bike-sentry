#define PIN_LED 1
#define PIN_BUTTON 2
#define PIN_PIEZO 3
#define PIN_BUZZER 4

#define THRESHOLD_PIEZO 30
#define DELAY_BUTTON_DEBOUNCE_MS 50

// after piezo detects movement, wait for DELAY_PIEZO_MOVED_MS and check if it is actually a button press
#define DELAY_PIEZO_MOVED_MS 500
enum State {
  DEEP_SLEEP,
  SENTRY,
  ATTENTION,
  ALARM
};

unsigned long lastButtonPressTime = 0;

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
      digitalWrite(PIN_LED, LOW);
      if (button_pressed()) {
        state = SENTRY;
        toggle(PIN_LED, 5, 100);
      }
      break;
    case SENTRY:
      if (piezo_moved()) state = ATTENTION;
      if (button_pressed()) state = DEEP_SLEEP;
      break;
    case ATTENTION:
      digitalWrite(PIN_LED, HIGH);
      if (button_pressed()) state = DEEP_SLEEP;
      break;
    case ALARM:
      break;
  }
}

bool button_pressed() {
  static bool lastButtonState = LOW;
  static unsigned long lastDebounceTime = 0;
  static bool buttonState = LOW;

  bool currentReading = digitalRead(PIN_BUTTON);

  if (currentReading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DELAY_BUTTON_DEBOUNCE_MS) {
    if (currentReading != buttonState) {
      buttonState = currentReading;
      if (buttonState) {
        lastButtonState = currentReading;
        return true;
      }
    }
  }

  lastButtonState = currentReading;
  return false;
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
