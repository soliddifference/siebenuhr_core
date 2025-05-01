#pragma once

#include "siebenuhr_core.h"

namespace siebenuhr_core {

    class UIButton {

    public:
        UIButton(uint8_t buttonPin);
        ~UIButton() {};

        void registerCallbacks(void (*callbackFncButton)(void));

        void update();
        void callbackButton(void);

        bool getState();
        bool isPressed(int countThreshold=1);
        bool isReleased(int countThreshold=1);
        int getTimeSinceStateChange();

    private:
        uint8_t _nButtonPin;
        bool _bCurrentState;
        int _nUpdateCounter;
        int _nButtonStateChangeTime;
    };

}
