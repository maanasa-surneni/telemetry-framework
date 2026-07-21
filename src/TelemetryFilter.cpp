#include "TelemetryFilter.hpp"

TelemetryFilter::TelemetryFilter(
    double minTemp,
    double maxTemp
)
    : minTemp_(minTemp),
    maxTemp_(maxTemp)

{

}

bool TelemetryFilter::accepts(
    const TelemetryMessage& message
) const
{
    const double temp = message.getTemperature();

    const int battery = message.getBatteryPercentage();

    const auto timestamp = message.getTimeStamp();

    const bool tempIsValid = temp >= minTemp_ && temp <= maxTemp_;

    const bool batteryIsValid = battery >= 0 && battery <= 100;

    const bool timestampIsValid = timestamp > 0;

    return tempIsValid && batteryIsValid && timestampIsValid;
}