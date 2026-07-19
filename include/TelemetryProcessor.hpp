#pragma once

#include <cstddef>

#include "TelemetryMessage.hpp"

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
    
    private:
        std::size_t processedCount_;
        std::size_t invalidCount_;

        double tempSum_;
        double minTemp_;
        double maxTemp_;
};