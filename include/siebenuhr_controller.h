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
    };
}