#include <iostream>

#include "SensorSimulator.hpp"
#include "TelemetryQueue.hpp"

int main()
{
    SensorSimulator simulator("sensor-01");

    // The queue can store up to 10 messages.
    TelemetryQueue queue(10);

    // TelemetryMessage has no default constructor, so create it using the simulator.
    TelemetryMessage message = simulator.generateMessage();

    for (int i{0}; i < 5; ++i) {
        message = simulator.generateMessage();

        if (message.isValid()) {
            if (!queue.tryPush(message)) {
                std::cout << "Queue is full. Message was not added."
                          << std::endl;
            }
        }
    }

    std::cout << "Queue contains "
              << queue.size()
              << " messages."
              << std::endl;

    // tryPop replaces 'message' with the oldest message in the queue.
    while (queue.tryPop(message)) {
        message.print();

        std::cout << "Messages remaining: "
                  << queue.size()
                  << std::endl;
    }

    return 0;
}