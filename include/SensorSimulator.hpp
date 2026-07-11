#pragma once

#include "TelemetryMessage.hpp"

#include <random>
#include <string>

class SensorSimulator{
    public: 
        explicit SensorSimulator(std::string deviceId);

        TelemetryMessage generateMessage(); //generateMessage creates and returns one TelemetryMessage
    
    private:
        std::string deviceId_;
        int batteryPercentage_;

        std::mt19937 randomEngine_; //randomEngine_ generates random values
        std::uniform_real_distribution<double> temperatureDistribution_; //temperatureDistribution limits temperature to a realistic range
};