#include <iostream>
#include <vector>

#include "SensorSimulator.hpp"

int main()
{
    SensorSimulator simulator("sensor-01");

    //creates an initially empty collection that can hold many TelemetryMessage objects
    std::vector<TelemetryMessage> messages;

    for (int i{0}; i<10; ++i){
        TelemetryMessage message = simulator.generateMessage();

        if (message.isValid()){
            //the push_back adds a message to the end of the vector
            messages.push_back(message);
        } else {
            std::cout << "Invalid message rejected." << std::endl;
        }
    }

    std::cout << "Store messages: " << messages.size() << std::endl;

    //loops through every message in the vector
    for (const TelemetryMessage& message : messages){
        message.print();
        std::cout << std::endl;
    }

    return 0;

}