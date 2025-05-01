#include "siebenuhr_button.h"

namespace siebenuhr_core
{
    UIButton::UIButton(uint8_t buttonPin)
    {
        _bCurrentState = false;
        _nButtonPin = buttonPin;
        _nButtonStateChangeTime = millis();
        _nUpdateCounter = 999;

        pinMode(constants::LED2_PIN, OUTPUT);
        pinMode(_nButtonPin, INPUT);
    }

    void UIButton::registerCallbacks(void (*callbackFncButton)(void))
    {
        attachInterrupt(digitalPinToInterrupt(this->_nButtonPin), callbackFncButton, CHANGE);
    }

    void UIButton::update()
    {
        _nUpdateCounter++;
        if (isPressed()) {
            digitalWrite(constants::LED2_PIN, HIGH);
        } else {
            digitalWrite(constants::LED2_PIN, LOW);
        }
    }

    void IRAM_ATTR UIButton::callbackButton()
    {
        _bCurrentState = digitalRead(_nButtonPin); // LOW = pressed & HIGH = not pressed, because internal pull-up resistor is enabled
        _nUpdateCounter = 0;
        _nButtonStateChangeTime = millis();
    }

    bool UIButton::getState()
    {
        return _bCurrentState;
    }

    bool UIButton::isPressed(int countThreshold)
    {
        return (getState() && _nUpdateCounter <= countThreshold);
    }

    bool UIButton::isReleased(int countThreshold)
    {
        return (!getState() && _nUpdateCounter <= countThreshold);
    }

    int UIButton::getTimeSinceStateChange()
    {
        return millis() - _nButtonStateChangeTime;
    }
    
}