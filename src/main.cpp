#include <iostream>
#include <vector>

#include "SensorSimulator.hpp"
#include "TelemetryProcessor.hpp"
#include "CsvExporter.hpp"

int main()
{
    SensorSimulator simulator("sensor-01");
    TelemetryProcessor processor;
    CsvExporter exporter;

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

    std::cout << "Stored messages: " << messages.size() << std::endl;

    if (!messages.empty()){
        std::cout << "Telemetry statistics" << std::endl;
        std::cout << "Average temperature: " << processor.calculateAverageTemperature(messages) << " C" << std::endl;
        std::cout << "Minimum temperature: " << processor.findMinimumTemperature(messages) << " C" << std::endl;
        std::cout << "Maximum temperature: " << processor.findMaximumTemperature(messages) << " C" << std::endl;
        std::cout << "Average battery: " << processor.calculateAverageBattery(messages) << " %" << std::endl;

        exporter.exportMessages(
            messages,
            "telemetry_data.csv"
        );

        std::cout << "Telemetry exported to telemetry_data.csv" << std::endl;
    }

    return 0;
}