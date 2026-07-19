#include "CsvExporter.hpp"

#include <fstream>
#include <stdexcept>

CsvExporter::CsvExporter(const std::string& filename) 
    : file_(filename) 
{
    if (file_.is_open()) {
        file_ << "device_id,timestamp,temperature,battery_percentage" << std::endl;
    }
}

bool CsvExporter::isOpen() const {
    return file_.is_open();
}

void CsvExporter::write(const TelemetryMessage& message) {
    if (!file_.is_open()) {
        return;
    }

    file_ << message.getDeviceId() << ","
          << message.getTimeStamp() << ","
          << message.getTemperature() << ","
          << message.getBatteryPercentage() << std::endl;
}