#include <Timer.h>
#include "motor.h"

/**
 * Notities:
 *  - Buttons worden verwacht opgezet te worden als INPUT_PULLUP, oftewel LOW betekent ingedrukt. 
 *    https://docs.arduino.cc/learn/microcontrollers/digital-pins#properties-of-pins-configured-as-input_pullup
 */

Timer t;

int registerBlink(int pin, int type);

const int leftBlinkerButtonPin = 10;
const int leftBlinkerLedPin =  13;
const int rightBlinkerButtonPin = 6;
const int rightBlinkerLedPin =  5;
const int lightSwitchLed = 2;
const int lightSwitchButton = 4;
const int bigLightLed = 8;
const int bigLightButton = 3;

ButtonPulseControlledLed leftBlinker(leftBlinkerLedPin, leftBlinkerButtonPin, &t, registerBlink);
ButtonPulseControlledLed rightBlinker(rightBlinkerLedPin, rightBlinkerButtonPin, &t, registerBlink);
ButtonControlledStatefulLed lightSwitch(lightSwitchLed, lightSwitchButton);
ButtonControlledDualStatefulLed bigLightSwitch(bigLightLed, bigLightButton);

void setup() {
  Serial.begin(9600);

  leftBlinker.setup();
  rightBlinker.setup();
  lightSwitch.setup();
  bigLightSwitch.setup();
}

void loop() {
  leftBlinker.checkPulse();
  rightBlinker.checkPulse();
  lightSwitch.checkButton();
  bigLightSwitch.checkButton();
  t.update();
}

int registerBlink(int pin, int type) 
{
  if (type == 2) {
    if (pin == leftBlinkerLedPin) rightBlinker.emergencyBlinking();
    if (pin == rightBlinkerLedPin) leftBlinker.emergencyBlinking();
    return t.oscillate(pin, 400, LOW, -1);
  }
  if (pin == leftBlinkerLedPin) rightBlinker.stopBlinking();
  if (pin == rightBlinkerLedPin) leftBlinker.stopBlinking();
  if (type == 0) return t.oscillate(pin, 400, LOW, 3);
  if (type == 1) return t.oscillate(pin, 400, LOW, 30);
  return -1;
}
