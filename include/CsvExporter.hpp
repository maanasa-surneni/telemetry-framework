#pragma once

#include "TelemetryMessage.hpp"

#include <string>
#include <fstream>

class CsvExporter {
    public:
        explicit CsvExporter(const std::string& filename);

        bool isOpen() const;
        void write(const TelemetryMessage& message);
    
    private:
        //std::ofstream means output file stream; it allows program to create and write to a file
        std::ofstream file_;
};

