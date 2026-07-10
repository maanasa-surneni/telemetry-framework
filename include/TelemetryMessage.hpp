//this file describes what the class contains and what it can do

#pragma once

#include <cstdint>
#include <string>

//define a class called TelemetryMessage
class TelemetryMessage {
    public: //things outside code is allowed to use
        TelemetryMessage(
            std::string deviceId,
            std::int64_t timestamp,
            double temperature,
            int batteryPercentage
        );

        void print() const;
        bool isValid() const;

    private: //data stored inside the object (all the values listed under are objects)
        std::string deviceId_;
        std::int64_t timestamp_;
        double temperature_;
        int batteryPercentage_;
};