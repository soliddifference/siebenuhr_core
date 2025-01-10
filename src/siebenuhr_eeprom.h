#pragma once

namespace siebenuhr_core
{
    class Configuration
    {
    public:
        Configuration(/* args */);
        ~Configuration();

        static int loadBrightness();
        static void saveBrightness(int brightness);

    private:
        /* data */
    };
}