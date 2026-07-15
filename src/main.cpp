#include <iostream>
#include <vector>

#include "SensorSimulator.hpp"
#include "TelemetryQueue.hpp"

int main()
{
    SensorSimulator simulator("sensor-01");
    TelemetryQueue queue;

    for (int i{0}; i<5; ++i){
        TelemetryMessage message = simulator.generateMessage();

        if (message.isValid()){
            queue.push(message);
        }
    }

    std::cout << "Queue contains " << queue.size() << " messages." << std::endl;

    while (!queue.empty()){
        TelemetryMessage message = queue.pop();

        message.print();

        std::cout << "Message remaining: " << queue.size() << std::endl;
    }

    return 0;
}