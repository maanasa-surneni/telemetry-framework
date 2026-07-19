#include "TelemetryAlert.hpp"

#include <sstream>

TelemetryAlert::TelemetryAlert(
    double highTempThreshold,
    int lowBatteryThreshold
)

    : highTempThreshold_(highTempThreshold),
    lowBatteryThreshold_(lowBatteryThreshold)
{
}

bool TelemetryAlert::hasHighTemp(const TelemetryMessage& message) const {
    return message.getTemperature() > highTempThreshold_;
}

bool TelemetryAlert::hasLowBattery(const TelemetryMessage& message) const {
    return message.getBatteryPercentage() < lowBatteryThreshold_; 
}

std::string TelemetryAlert::createAlertMessage(const TelemetryMessage& message) const {
    std::ostringstream alert;

    if (hasHighTemp(message)) {
        alert << "HIGH TERMPERATURE: "
              << message.getDeviceId()
              << " reported "
              << message.getBatteryPercentage()
              << "%";
    }

    return alert.str();
}