#pragma once

namespace siebenuhr_core
{
    class Controller 
    {
    public:
    	static Controller* getInstance();

        void setInterval(int interval);
        void setLEDPin(int pin);

        void initialize();
        void update();

    private:
        Controller() = default;

    	static Controller* s_instance;

        int m_ledPin;
        unsigned long m_previousMillis;
        unsigned long m_interval;
        bool m_ledState;
    };
}