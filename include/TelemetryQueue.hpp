#pragma once

#include "TelemetryMessage.hpp"

#include <cstddef>
#include <queue>
#include <mutex>
#include <condition_variable>

class TelemetryQueue {
    public:
        explicit TelemetryQueue(std::size_t capacity);

        //non-blocking operations
        bool tryPush(const TelemetryMessage& message);
        bool tryPop(TelemetryMessage& message);
        
        //blocking operations
        bool waitPush(const TelemetryMessage& message);
        bool waitPop(TelemetryMessage& message);

        //stops the queue and wakes waiting threads
        void stop();

        bool empty() const;
        
        std::size_t size() const;
    
    private:
        //here std::queue creates a queue that stores TelemetryMessage objects
        //this is private since other code should not directly manipulate the internal queue
        std::queue<TelemetryMessage> queue_;
        std::size_t capacity_;
        //A mutex protects shared data so that only one thread can access a critical section at a time
        mutable std::mutex mutex_;

        //this condition means consumers wait until the queue is not empty
        std::condition_variable notEmpty_;
        //this condition means producers wait until the queue is not full
        std::condition_variable notFull_;

        //tells the waiting threads that the queue is shutting down
        bool stopped_{false};
};