#pragma once

namespace siebenuhr_core
{
    class Controller 
    {
    public:
    	static Controller* getInstance();

        void initialize();
        void update();

    private:
        Controller() = default;

    	static Controller* s_instance;

        unsigned long m_previousMillis;
        unsigned long m_interval;
        bool m_ledState;
    };
}