#include "TelemetryQueue.hpp"

#include <stdexcept>

TelemetryQueue::TelemetryQueue(std::size_t capacity) 
    : capacity_(capacity) {
}

bool TelemetryQueue::tryPush(const TelemetryMessage& message) {
    {
        //locks the mutex when created and automatically unlocks it when the function exits
        std::lock_guard<std::mutex> lock(mutex_);

        if (stopped_ || queue_.size() >= capacity_){
            return false;
        }

        //adds a copy of the message to the back of the internal queue
        queue_.push(message);
    }
    
    //wakes one waiting consumer
    notEmpty_.notify_one();
    return true;
}

bool TelemetryQueue::tryPop(TelemetryMessage& message){
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (queue_.empty()){
            return false;
        }
        //copies the oldest message into the caller-provided object
        message = queue_.front();
        queue_.pop(); //removes the first item from the queue
    }

    //wakes one waiting producer
    notFull_.notify_one();
    return true;
}

bool TelemetryQueue::waitPush(const TelemetryMessage& message) {
    //the condition variable needs to temporarily unlock the mutex while waiting
    std::unique_lock<std::mutex> lock(mutex_);

    //wait until the queue has available space, or until the queue has been stopped
    notFull_.wait(lock, [this] {
        return queue_.size() < capacity_ || stopped_;
    });

    if (stopped_) {
        return false;
    }

    queue_.push(message);

    lock.unlock();
    notEmpty_.notify_one();

    return true;
}

bool TelemetryQueue::waitPop(TelemetryMessage& message) {
    std::unique_lock<std::mutex> lock(mutex_);

    //wait until there is a message or until the queue has stopped
    notEmpty_.wait(lock, [this] {
        return !queue_.empty() || stopped_;
    });

    if(queue_.empty()) {
        return false;
    }

    message = queue_.front();
    queue_.pop();

    lock.unlock();
    notFull_.notify_one();

    return true;
}

void TelemetryQueue::stop() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stopped_ = true;
    }

    //notify_all since when the queue stops, every waiting thread should wake up
    notEmpty_.notify_all();
    notFull_.notify_all();
}

bool TelemetryQueue::empty() const{
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

std::size_t TelemetryQueue::size() const{
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}