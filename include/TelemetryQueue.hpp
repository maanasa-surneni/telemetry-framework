#pragma once

#include "TelemetryMessage.hpp"

#include <cstddef>
#include <queue>

class TelemetryQueue {
    public:
        void push(const TelemetryMessage& message);

        TelemetryMessage pop();

        bool empty() const;

        //std::size_t is an unsigned int type (only represents positive values)
        std::size_t size() const;
    
    private:
        //here std::queue creates a queue that stores TelemetryMessage objects
        //this is private since other code should not directly manipulate the internal queue
        std::queue<TelemetryMessage> messages_;
};