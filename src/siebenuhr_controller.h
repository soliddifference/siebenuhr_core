#pragma once

#include "siebenuhr_display.h"

namespace siebenuhr_core
{
    class Controller 
    {
    public:
    	static Controller* getInstance();

        void setInterval(int interval);
        void setLEDPin(int pin);

        void initialize(int clock_type);
        void update();

    private:
        Controller() = default;

    	static Controller* s_instance;

       	Display* m_display;

        // heartbeat
        int m_ledPin;
        unsigned long m_previousMillis;
        unsigned long m_interval;
        bool m_ledState;
    };
}