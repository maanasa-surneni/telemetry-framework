#pragma once

#include "TelemetryMessage.hpp"

class TelemetryFilter {
    public:
        TelemetryFilter(
            double minTemp,
            double maxTemp
        );

        bool accepts(
            const TelemetryMessage& message
        ) const;
    
    private:
        double minTemp_;
        double maxTemp_;
};