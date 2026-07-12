#include "TelemetryProcessor.hpp"

#include <stdexcept>

double TelemetryProcessor::calculateAverageTemperature(
    const std::vector<TelemetryMessage>& messages
) const

{
    if (messages.empty()){
        throw std::invalid_argument("Cannot process an empty message list.");
    }

    double totalTemperature = 0.0;

    for (const TelemetryMessage& message : messages){
        totalTemperature += message.getTemperature();
    }

    return totalTemperature / static_cast<double>(messages.size());
}

double TelemetryProcessor::findMinimumTemperature(
    const std::vector<TelemetryMessage>& messages
) const
{
    if (messages.empty()){
        throw std::invalid_argument("Cannot process an empty message list.");
    }

    double minimumTemperature = messages.front().getTemperature();

    for (const TelemetryMessage& message : messages){
        if (message.getTemperature() < minimumTemperature){
            minimumTemperature = message.getTemperature();
        }
    }
    return minimumTemperature;
}

double TelemetryProcessor::findMaximumTemperature(
    const std::vector<TelemetryMessage>& messages
) const
{
    if (messages.empty()){
        throw std::invalid_argument("Cannot process an empty message list.");
    }

    double maximumTemperature = messages.front().getTemperature();

    for (const TelemetryMessage& message : messages){
        if (message.getTemperature() > maximumTemperature){
            maximumTemperature = message.getTemperature();
        }
    }
    return maximumTemperature;
}

double TelemetryProcessor::calculateAverageBattery(
    const std::vector<TelemetryMessage>& messages
) const
{
    if (messages.empty()){
        throw std::invalid_argument("Cannot process an empty message list.");
    }

    int totalBattery = 0;

    for (const TelemetryMessage& message : messages){
        totalBattery += message.getBatteryPercentage();
    }

    return static_cast<double>(totalBattery) / static_cast<double>(messages.size());
}