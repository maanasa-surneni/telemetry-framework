#include "CsvExporter.hpp"

#include <fstream>
#include <stdexcept>

void CsvExporter::exportMessages(
    const std::vector<TelemetryMessage>& messages,
    const std::string& filePath 
) const 
{
    //this creates an output file stream
    std::ofstream outputFile(filePath);

    if (!outputFile.is_open()){
        throw std::runtime_error(
            "Could not open CSV file: " + filePath
        );
    }

    //creates the csv header (where each row will contain one telemetry message)
    outputFile << "device_id,timestamp,temperature,battery_percentage" << std::endl;

    for (const TelemetryMessage& message : messages){
        outputFile
            << message.getDeviceId() << ","
            << message.getTimeStamp() << ","
            << message.getTemperature() << ","
            << message.getBatteryPercentage() << std::endl;
    }
}