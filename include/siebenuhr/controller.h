#pragma once

namespace siebenuhr::core
{
    class Controller 
    {
    public:
        Controller() = default;
        ~Controller() = default;

        void initialize();
        void update();
    };
}