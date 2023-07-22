#include <Arduino.h>
#include <Timer.h>

class ButtonControlledLed {
public:
    ButtonControlledLed(int ledPin, int buttonPin);
    void setup(void);
    int ledPin() const { return _ledPin; }
protected:
    bool _buttonActive = false;
    int _ledPin;
    int _buttonPin;
};

class ButtonPulseControlledLed : public ButtonControlledLed
{
public:
    ButtonPulseControlledLed(int ledPin, int buttonPin, Timer *timer, int (*pulseAction)(int, int));
    void checkPulse(void);
    void stopBlinking(void);
    void emergencyBlinking(void);
protected:
    unsigned long _buttonTimer = 0;
    Timer *_timer;
    int _eventIndex = -1;
    int (*_pulseAction)(int, int); 
};

class ButtonControlledDualStatefulLed : public ButtonControlledLed
{
public:
    ButtonControlledDualStatefulLed(int ledPin, int buttonPin);
    void checkButton(void);
private:
    unsigned long _buttonTimer = 0;
    unsigned long _debounceTime;
    int _state = 0;
};

class ButtonControlledStatefulLed : public ButtonControlledLed
{
public:
    ButtonControlledStatefulLed(int ledPin, int buttonPin);
    void checkButton();
private:
    unsigned long _debounceTime;
    int _state = 0;
};
