#include "siebenuhr_button.h"

/*
    How to use:

    button.update();

    if (button.getClickType() == ClickType::Single) {
        // trigger single-click action
    }

    if (button.getClickType() == ClickType::Double) {
        // trigger double-click action
    }

    if (button.isLongPress()) {
        // hold active — repeat behavior
    }

    if (button.isReleased()) {
        // detect release to stop long press or trigger something else
    }

*/


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
        int reading = digitalRead(m_buttonPin);
        unsigned long now = millis();

        m_pressedEvent = false;
        m_releasedEvent = false;
        m_clickType = ClickType::None;

        if (reading != m_lastState) {
            m_lastDebounceTime = now;
        }

        if ((now - m_lastDebounceTime) > DEBOUNCE_DELAY) {
            if (reading != m_state) {
                m_state = reading;

                if (m_state == LOW) { // button down
                    m_pressStartTime = now;
                    m_pressedEvent = true;

                    if (m_waitingForSecondClick && (now - m_lastPressTime) <= DOUBLE_CLICK_TIME) {
                        m_clickType = ClickType::Double;
                        m_waitingForSecondClick = false;
                    } else {
                        m_lastPressTime = now;
                        m_waitingForSecondClick = true;
                    }

                } else { // button up (release)
                    m_lastReleaseTime = now;
                    m_releasedEvent = true;

                    if (m_waitingForSecondClick && (now - m_lastPressTime) > DOUBLE_CLICK_TIME) {
                        m_clickType = ClickType::Single;
                        m_waitingForSecondClick = false;
                    }

                    m_longPressActive = false; // reset long press on release
                }
            }

            // Long press check (held down)
            if (m_state == LOW && (now - m_pressStartTime) >= LONG_PRESS_THRESHOLD) {
                m_longPressActive = true;
            }
        }

        m_lastState = reading;
        digitalWrite(m_ledPin, (m_state == LOW) ? HIGH : LOW);
    }

    ClickType UIButton::getClickType()
    {
        return m_clickType;
    }

    bool UIButton::isLongPress()
    {
        return m_longPressActive;
    }

    bool UIButton::isPressed()
    {
        return m_pressedEvent;
    }

    bool UIButton::isReleased()
    {
        return m_releasedEvent;
    }
}