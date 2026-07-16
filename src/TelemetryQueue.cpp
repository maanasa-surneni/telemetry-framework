#include "TelemetryQueue.hpp"

#include <stdexcept>

TelemetryQueue::TelemetryQueue(std::size_t capacity) 
    : capacity_(capacity) {
    if (capacity == 0){
        throw std::invalid_argument(
            "TelemetryQueue capacity must be greater than zero"
        );
    }
}

bool TelemetryQueue::tryPush(const TelemetryMessage& message) {

    //locks the mutex when created and automatically unlocks it when the function exits
    std::lock_guard<std::mutex> lock(mutex_);

    if (queue_.size() >= capacity_) {
        return false;
    }

    //adds a copy of the message to the back of the internal queue
    queue_.push(message);
    return true;
}

bool TelemetryQueue::tryPop(TelemetryMessage& message){
    std::lock_guard<std::mutex> lock(mutex_);

    if (queue_.empty()){
        return false;
    }
    //copies the oldest message into the caller-provided object
    message = queue_.front();
    queue_.pop(); //removes the first item from the queue

    return true;
}

bool TelemetryQueue::empty() const{
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

std::size_t TelemetryQueue::size() const{
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}