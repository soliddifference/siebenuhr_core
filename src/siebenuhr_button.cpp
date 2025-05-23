#include "siebenuhr_button.h"

/*
    How to use:

    ButtonState state = button.update();

    if (state == ButtonState::SingleClick) {
        // trigger single-click action
    }

    if (state == ButtonState::DoubleClick) {
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

        // pinMode(m_ledPin, OUTPUT);
        pinMode(m_buttonPin, INPUT_PULLUP);
        m_lastTransition = millis();
    }

    ButtonState UIButton::update()
    {
        int reading = digitalRead(m_buttonPin);
        unsigned long now = millis();

        // Reset event flags
        m_pressedEvent = false;
        m_releasedEvent = false;
        m_buttonState = ButtonState::Idle;

        // Debounce logic
        if (reading != m_lastState) {
            m_lastTransition = now;
        }

        // delayed event: if time since last single click is greater than double click threshold, fire single click event
        if (m_lastSingleClickTime != 0 && (now - m_lastSingleClickTime) > DOUBLE_CLICK_TIME) {
            m_lastSingleClickTime = 0;
            m_buttonState = ButtonState::SingleClick;
            // logMessage(LOG_LEVEL_INFO, "Button SINGLE-Click");
        }

        if (m_lastPressEventTime != 0) {
            if ((now - m_lastPressEventTime) > LONG_PRESS_THRESHOLD) {
                // logMessage(LOG_LEVEL_INFO, "Button long pressed");
                m_buttonState = ButtonState::LongPress;
            }
        }

        if ((now - m_lastTransition) > DEBOUNCE_DELAY) {
            if (reading != m_state) {
                m_state = reading;

                if (m_state == LOW) { // button down
                    m_pressedEvent = true;
                    m_lastReleaseEventTime = 0;
                    m_lastPressEventTime = now; // register time of press event to check for long press
                } 
                else 
                { // button up (release)
                    m_releasedEvent = true;
                    m_lastReleaseEventTime = now;
                    m_lastPressEventTime = 0; // reset press event time to avoid false long press detection

                    if (m_buttonState != ButtonState::LongPress) {
                        if (m_lastSingleClickTime == 0) {
                            // delay firing single click-event, wait if double click is detected
                            // logMessage(LOG_LEVEL_INFO, "check if next click is within DC threshold!");
                            m_lastSingleClickTime = now;
                        } else {
                            // logMessage(LOG_LEVEL_INFO, "Button DOUBLE-Click");
                            m_buttonState = ButtonState::DoubleClick;
                            m_lastSingleClickTime = 0;
                        }
                    } else {
                        // logMessage(LOG_LEVEL_INFO, "Button LONG-Click");
                        m_buttonState = ButtonState::LongClick;
                    }
                }
            }
        }

        m_lastState = reading;
        // digitalWrite(m_ledPin, (m_state == LOW) ? HIGH : LOW);

        return m_buttonState;
    }

    ButtonState UIButton::getState()
    {
        return m_buttonState;
    }

    bool UIButton::isLongPress()
    {
        return m_buttonState == ButtonState::LongPress;
    }

    bool UIButton::isPressed()
    {
        return m_pressedEvent;
    }

    bool UIButton::isReleased()
    {
        return m_releasedEvent;
    }

    unsigned long UIButton::getLastReleaseEventTime()
    {
        return m_lastReleaseEventTime;
    }
}