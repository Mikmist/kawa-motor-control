#include "motor.h"

ButtonControlledLed::ButtonControlledLed(int ledPin, int buttonPin) {
    _ledPin = ledPin;
    _buttonPin = buttonPin;
}

void ButtonControlledLed::setup(void) {
    pinMode(_ledPin, OUTPUT);
    pinMode(_buttonPin, INPUT_PULLUP);
}

ButtonPulseControlledLed::ButtonPulseControlledLed(int ledPin, int buttonPin, Timer *timer, int (*pulseAction)(int, int)) 
      : ButtonControlledLed(ledPin, buttonPin) {
    _timer = timer;
    _pulseAction = *pulseAction;
}

void ButtonPulseControlledLed::checkPulse(void) {
  if (digitalRead(_buttonPin) == LOW) {
    if (_buttonActive == false) {
      _buttonActive = true;
      _buttonTimer = millis();
    }
  } else if (_buttonActive == true) {
    stopBlinking();
    if (millis() - _buttonTimer < 500) _eventIndex = (*_pulseAction)(_ledPin, 0);
    else if (millis() - _buttonTimer > 1500) _eventIndex = (*_pulseAction)(_ledPin, 2); 
    else _eventIndex = (*_pulseAction)(_ledPin, 1); 
    _buttonActive = false;
  }
}

void ButtonPulseControlledLed::stopBlinking(void) {
    if (_eventIndex == -1) return;
    (*_timer).stop(_eventIndex);
    digitalWrite(_ledPin, LOW);
    _eventIndex = -1;
}

void ButtonPulseControlledLed::emergencyBlinking() {
  stopBlinking();
  _eventIndex = (*_timer).oscillate(_ledPin, 400, LOW, -1);
}

ButtonControlledStatefulLed::ButtonControlledStatefulLed(int ledPin, int buttonPin) : ButtonControlledLed(ledPin, buttonPin) {}

void ButtonControlledStatefulLed::checkButton() {
    if (digitalRead(_buttonPin) == LOW) {
        _buttonActive = true;
        _debounceTime = millis();
    }
    if (digitalRead(_buttonPin) == HIGH && _buttonActive && millis() - _debounceTime > 50) {
        _buttonActive = false;
        _state = !_state;
        digitalWrite(_ledPin, _state);
    }
}

ButtonControlledDualStatefulLed::ButtonControlledDualStatefulLed(int ledPin, int buttonPin) 
      : ButtonControlledLed(ledPin, buttonPin) {}

void ButtonControlledDualStatefulLed::checkButton() {
    if (digitalRead(_buttonPin) == LOW) {
        if (!_buttonActive) _buttonTimer = millis();
        _buttonActive = true;
        _debounceTime = millis();
    }
    // Flip flop state of the button.
    if (_buttonActive) {
      digitalWrite(_ledPin, HIGH);
    }
    if (digitalRead(_buttonPin) == HIGH && _buttonActive && millis() - _debounceTime > 50) {
        _buttonActive = false;
        if ( millis() - _buttonTimer > 1000) {
          _state = !_state;
          digitalWrite(_ledPin, _state);
        } else {
          digitalWrite(_ledPin, LOW);
        }
    }
}