#pragma once

#include "siebenuhr_core.h"

namespace siebenuhr_core {

    class UIButton {

    public:
        UIButton(uint8_t buttonPin, uint8_t ledPin);
        ~UIButton() {};

        // void registerCallbacks(void (*callbackFncButton)(void));

        void update();
        // void callbackButton(void);

        bool getState();
        bool isPressed();
        bool isReleased();

        int getTimeSinceStateChange();

    private:
        uint8_t m_buttonPin;
        uint8_t m_ledPin;
        // bool _bCurrentState;
        // int _nUpdateCounter;

        int m_state = HIGH;
        int m_lastState = HIGH;
        unsigned long m_lastDebounceTime = 0;
        bool m_pressedEvent = false;    

        // int m_stateChangeTime;
    };

}
