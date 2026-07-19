#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>

#include "TelemetryMessage.hpp"

//a struct lets us group related values together
struct DeviceStatistics {
    std::size_t processedCount{0};
    double tempSum{0.0};
    double minTemp{0.0};
    double maxTemp{0.0};
};

class TelemetryProcessor {
    public:
        TelemetryProcessor();

        //this function accepts one telemetry message and updates the statistics
        void process(const TelemetryMessage& message);

        std::size_t getProcessedCount() const;
        std::size_t getInvalidCount() const;

        double getAverageTemp() const;
        double getMinTemp() const;
        double getMaxTemp() const;

        bool hasDevice(const std::string& deviceId) const;

        const DeviceStatistics& getDeviceStatistics(
            const std::string& deviceId
        ) const;

        double getDeviceAverageTemp(
            const std::string& deviceId
        ) const;
    
    private:
        std::size_t processedCount_;
        std::size_t invalidCount_;

        double tempSum_;
        double minTemp_;
        double maxTemp_;

        std::unordered_map<std::string, DeviceStatistics> deviceStatistics_;
};