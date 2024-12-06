#include "siebenuhr_core.h"

S7Controller* S7Controller::s_instance = nullptr;

S7Controller* S7Controller::getInstance()
{
    if (S7Controller::s_instance == nullptr) 
    {
        S7Controller::s_instance = new S7Controller();
    }
    return S7Controller::s_instance;
}

void S7Controller::initialize() 
{
    // just some basic code to test the library integration
}

void S7Controller::update() 
{
    // just some basic code to test the library integration
}