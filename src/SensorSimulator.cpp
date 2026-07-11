#include "SensorSimulator.hpp"

#include <chrono>
#include <cstdint>
#include <utility>

SensorSimulator::SensorSimulator(std::string deviceId)
    : deviceId_(std::move(deviceId)),
      batteryPercentage_(100),
      randomEngine_(std::random_device{}()),
      temperatureDistribution_(18.0, 30.0)

{

}

TelemetryMessage SensorSimulator::generateMessage() {
    const auto now = std::chrono::system_clock::now();

    const auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(
        now.time_since_epoch()
    ).count();

    const double temperature = temperatureDistribution_(randomEngine_);

    if (batteryPercentage_ > 0){
        --batteryPercentage_;
    }

    return TelemetryMessage{
        deviceId_,
        static_cast<std::int64_t>(timestamp),
        temperature,
        batteryPercentage_
    };
}