#include "TelemetryMessage.hpp"

#include <iostream>
#include <utility>

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

void TelemetryMessage::print() const
{
    std::cout << "Device ID: " << deviceId_ << std::endl;
    std::cout << "Timestamp: " << timestamp_ << std::endl;
    std::cout << "Temperature: " << temperature_ << " C" << std::endl;
    std::cout << "Battery: " << batteryPercentage_ << "%" << std::endl;
}