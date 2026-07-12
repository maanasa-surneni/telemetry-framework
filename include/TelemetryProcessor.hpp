#pragma once

#include "TelemetryMessage.hpp"

#include <vector>

class TelemetryProcessor {
    public:
        double calculateAverageTemperature(
            const std::vector<TelemetryMessage>& messages
        ) const;

        double findMinimumTemperature(
            const std::vector<TelemetryMessage>& messages
        ) const;

        double findMaximumTemperature(
            const std::vector<TelemetryMessage>& messages
        ) const;

        double calculateAverageBattery(
            const std::vector<TelemetryMessage>& messages
        ) const;
};