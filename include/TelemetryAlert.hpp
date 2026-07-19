#pragma once

#include <string>

#include "TelemetryMessage.hpp"

class TelemetryAlert {
    public:
        TelemetryAlert(double highTempThreshold, int lowBatteryThreshold);

        bool hasHighTemp(const TelemetryMessage& message) const;

        bool hasLowBattery(const TelemetryMessage& message) const;

        std::string createAlertMessage(const TelemetryMessage& message) const;
    
    private:
        double highTempThreshold_;
        int lowBatteryThreshold_;
};