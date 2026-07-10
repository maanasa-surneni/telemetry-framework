//this file contains the actual implementations

#include "TelemetryMessage.hpp"

#include <iostream>
#include <utility>

//constructor declaration (runs automatically when an object is created)
TelemetryMessage::TelemetryMessage(
    std::string deviceId,
    std::int64_t timestamp,
    double temperature,
    int batteryPercentage
)

    : deviceId_(std::move(deviceId)),
      timestamp_(timestamp),
      temperature_(temperature),
      batteryPercentage_(batteryPercentage)

{
}

//this is a function that does not return a value
void TelemetryMessage::print() const
{
    std::cout << "Device ID: " << deviceId_ << std::endl;
    std::cout << "Timestamp: " << timestamp_ << std::endl;
    std::cout << "Temperature: " << temperature_ << " C" << std::endl;
    std::cout << "Battery: " << batteryPercentage_ << "%" << std::endl;
}

bool TelemetryMessage::isValid() const
{
    if (deviceId_.empty()){
        return false;
    }
    if (timestamp_ < 0){
        return false;
    }
    if (temperature_ < -100.0 || temperature_ > 200.0){
        return false;
    }
    if (batteryPercentage_ < 0 || batteryPercentage_ > 100) {
        return false;
    }

    return true;
}