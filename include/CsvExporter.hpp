#pragma once

#include "TelemetryMessage.hpp"

#include <string>
#include <vector>

class CsvExporter {
    public:
        void exportMessages(
            const std::vector<TelemetryMessage>& messages,
            const std::string& filepath
        ) const;
};

