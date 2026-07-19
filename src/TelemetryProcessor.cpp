#include "TelemetryProcessor.hpp"

#include <stdexcept>

TelemetryProcessor::TelemetryProcessor()
    : processedCount_(0),
    invalidCount_(0),
    tempSum_(0.0),
    minTemp_(0.0),
    maxTemp_(0.0)

{

}

void TelemetryProcessor::process(const TelemetryMessage& message) {
    if (!message.isValid()) {
        ++invalidCount_;
        return;
    }

    const double temp = message.getTemperature();

    if (processedCount_ == 0) {
        minTemp_ = temp;
        maxTemp_ = temp;
    } else {
        if (temp < minTemp_) {
            minTemp_ = temp;
        }

        if (temp > maxTemp_) {
            maxTemp_ = temp;
        }
    }

    tempSum_ += temp;
    ++processedCount_;

    DeviceStatistics& statistics = deviceStatistics_[message.getDeviceId()];

    if (statistics.processedCount == 0) {
        statistics.minTemp = temp;
        statistics.maxTemp = temp;
    } else {
        if (temp < statistics.minTemp) {
            statistics.minTemp = temp;
        }

        if (temp > statistics.maxTemp) {
            statistics.maxTemp = temp;
        }
    }

    statistics.tempSum += temp;
    ++statistics.processedCount;
}

std::size_t TelemetryProcessor::getProcessedCount() const {
    return processedCount_;
}

std::size_t TelemetryProcessor::getInvalidCount() const {
    return invalidCount_;
}

double TelemetryProcessor::getAverageTemp() const {
    if (processedCount_ == 0) {
        return 0.0;
    }

    return tempSum_ / static_cast<double>(processedCount_);
}

double TelemetryProcessor::getMinTemp() const {
    return minTemp_;
}

double TelemetryProcessor::getMaxTemp() const {
    return maxTemp_;
}

bool TelemetryProcessor::hasDevice(const std::string& deviceId) const {
    return deviceStatistics_.find(deviceId) != deviceStatistics_.end();
}

const DeviceStatistics& TelemetryProcessor::getDeviceStatistics(const std::string& deviceId) const {
    return deviceStatistics_.at(deviceId); //the .at() function looks up an existing entry
}

double TelemetryProcessor::getDeviceAverageTemp(const std::string& deviceId) const {
    const DeviceStatistics& statistics = deviceStatistics_.at(deviceId);

    if (statistics.processedCount == 0) {
        return 0.0;
    }

    return statistics.tempSum / static_cast<double>(statistics.processedCount);
}