#pragma once

#include <cstdint>
#include <string>

class TelemetryMessage {
    public: 
        TelemetryMessage(
            std::string deviceId,
            std::int64_t timestamp,
            double temperature,
            int batteryPercentage
        );

        void print() const;

    private: 
        std::string deviceId_;
        std::int64_t timestamp_;
        double temperature_;
        int batteryPercentage_;
};