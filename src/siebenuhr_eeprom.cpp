#include "siebenuhr_core.h"
#include "siebenuhr_eeprom.h"

namespace siebenuhr_core
{
    Configuration::Configuration(/* args */)
    {
    }
    
    Configuration::~Configuration()
    {
    }    

    int Configuration::loadBrightness()
    {
       	// TODO: _nBrightness = _inst->readFromEEPROM(EEPROM_ADDRESS_BRIGHTNESS);
        return constants::DefaultBrightness;
    }

    void Configuration::saveBrightness(int brightness)
    {
        // siebenuhr::Controller::getInstance()->writeToEEPROM(EEPROM_ADDRESS_BRIGHTNESS, value);
    }

}