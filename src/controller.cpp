#include "siebenuhr/controller.h"

namespace siebenuhr::core
{
    Controller* Controller::s_instance = nullptr;

    Controller* Controller::getInstance()
    {
        if (Controller::s_instance == nullptr) 
        {
            Controller::s_instance = new Controller();
        }
        return Controller::s_instance;
    }

    void Controller::initialize() 
    {
        // just some basic code to test the library integration
    }

    void Controller::update() 
    {
        // just some basic code to test the library integration
    }
}