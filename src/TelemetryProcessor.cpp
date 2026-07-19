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