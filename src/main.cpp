#include <iostream>
#include "TelemetryMessage.hpp"
#include "SensorSimulator.hpp"

int main()
{
    SensorSimulator simulator("sensor-01");

    for (int i{0}; i<5; ++i){
        TelemetryMessage message = simulator.generateMessage();

        message.print();

        if (message.isValid()){
            std::cout << "Message is valid." << std::endl;
        } else {
            std::cout << "Message is invalid." << std::endl;
        }
        std::cout << std::endl;
    }


    return 0;

}