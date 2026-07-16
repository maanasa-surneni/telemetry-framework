#pragma once

#include "TelemetryMessage.hpp"

#include <cstddef>
#include <queue>

class TelemetryQueue {
    public:
        explicit TelemetryQueue(std::size_t capacity);

        bool tryPush(const TelemetryMessage& message);
        bool tryPop(TelemetryMessage& message);
        
        bool empty() const;
        bool full() const;
        //std::size_t is an unsigned int type (only pos. values)
        std::size_t size() const;
        std::size_t capacity() const;
    
    private:
        //here std::queue creates a queue that stores TelemetryMessage objects
        //this is private since other code should not directly manipulate the internal queue
        std::queue<TelemetryMessage> queue_;
        std::size_t capacity_;
        //A mutex protects shared data so that only one thread can access a critical section at a time
        mutable std::mutex mutex_;

};