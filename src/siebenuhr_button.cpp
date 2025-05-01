#include "siebenuhr_button.h"

namespace siebenuhr_core
{
    UIButton::UIButton(uint8_t buttonPin, uint8_t ledPin)
    {
        m_buttonPin = buttonPin;
        m_ledPin = ledPin;

        pinMode(m_ledPin, OUTPUT);
        pinMode(m_buttonPin, INPUT_PULLUP); 
    }

    void UIButton::update()
    {
        int buttonReading = digitalRead(m_buttonPin);
        if (buttonReading != m_lastState) 
        { 
            m_lastDebounceTime = millis(); 
        }

        m_pressedEvent = false;
        if ((millis() - m_lastDebounceTime) > constants::BUTTON_DEBOUNCE_DELAY) {
            if (buttonReading != m_state) {
                m_state = buttonReading;
                if (m_state == LOW) { 
                    m_pressedEvent = true;
                }
            }
        }
        m_lastState = buttonReading;

        if (isPressed()) {
            digitalWrite(m_ledPin, HIGH);
        } else {
            digitalWrite(m_ledPin, LOW);
        }
    }

    bool UIButton::getState()
    {
        return m_pressedEvent;
    }

    bool UIButton::isPressed()
    {
        // return (getState() && _nUpdateCounter <= countThreshold);
        return getState();
    }

    bool UIButton::isReleased()
    {
        // return (!getState() && _nUpdateCounter <= countThreshold);
        return !getState();
    }    
}