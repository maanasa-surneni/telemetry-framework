#include "TelemetryQueue.hpp"

#include <stdexcept>

void TelemetryQueue::push(const TelemetryMessage& message) {
    //adds a copy of the message to the back of the internal queue
    messages_.push(message);
}

TelemetryMessage TelemetryQueue::pop(){
    if (messages_.empty()){
        throw std::runtime_error(
            "Cannot delete from an empty telemetry queue."
        );
    }
    //copies the oldest message into a local variable using front() which looks at the first item
    TelemetryMessage message = messages_.front();
    messages_.pop(); //removes the first item from the queue

    return message;
}

bool TelemetryQueue::empty() const{
    return messages_.empty();
}

std::size_t TelemetryQueue::size() const{
    return messages_.size();
}