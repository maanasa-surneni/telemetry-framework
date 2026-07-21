#include <iostream>
#include <chrono>
#include <thread>

#include "SensorSimulator.hpp"
#include "TelemetryQueue.hpp"
#include "TelemetryProcessor.hpp"
#include "CsvExporter.hpp"
#include "TelemetryAlert.hpp"
#include "TelemetryFilter.hpp"

int main()
{
    SensorSimulator sensorOne("sensor-01");
    SensorSimulator sensorTwo("sensor-02");
    SensorSimulator sensorThree("sensor-03");

    // The queue can store up to 10 messages.
    TelemetryQueue queue(10);
    TelemetryProcessor processor;
    CsvExporter exporter("telemetry.csv");
    TelemetryAlert alertSystem(28.0, 20);
    TelemetryFilter filter(-40.0, 85.0);

    if (!exporter.isOpen()) {
        std::cerr << "Failed to open telemetry.csv" << std::endl;
        return 1;
    }

    constexpr int messagesPerSensor{10};

    //lambda capture which means that the thread's lambda can access local variables from main() by reference
    std::thread producerOne([&] {
        for (int i{0}; i<messagesPerSensor; ++i){
            TelemetryMessage message = sensorOne.generateMessage();

            if (message.isValid()) {
                if (!queue.waitPush(message)) {
                    return;
                }

            }

            std::this_thread::sleep_for(
                std::chrono::milliseconds(100)
            );
        }
    });

    std::thread producerTwo([&] {
        for (int i{0}; i < messagesPerSensor; ++i) {
            TelemetryMessage message =
                sensorTwo.generateMessage();

            if (message.isValid()) {
                if (!queue.waitPush(message)) {
                    return;
                }
            }

            std::this_thread::sleep_for(
                std::chrono::milliseconds(150)
            );
        }
    });

    std::thread producerThree([&] {
        for (int i{0}; i < messagesPerSensor; ++i) {
            TelemetryMessage message =
                sensorThree.generateMessage();

            if (message.isValid()) {
                if (!queue.waitPush(message)) {
                    return;
                }
            }

            std::this_thread::sleep_for(
                std::chrono::milliseconds(200)
            );
        }
    });

    TelemetryMessage output("placeholder", 0, 0.0, 0);

    std::thread consumer([&] {
        while (queue.waitPop(output)) {
            if (!filter.accepts(output)) {
                std::cout << "Rejected invalid message from " << output.getDeviceId() << std::endl;

                continue; //this is so that it skips through the rest of the loop iteration and messages do not reach the processor, exporter, and alert system
            }

            processor.process(output); //every consumed message is passed into this
            exporter.write(output);
            
            const std::string alert = alertSystem.createAlertMessage(output);

            if (!alert.empty()) {
                std::cout << "ALERT: " << alert << std::endl;
            }
        }

        std::cout << "Consumer finished processing messages." << std::endl;
    });

    producerOne.join();
    producerTwo.join();
    producerThree.join();

    queue.stop();

    consumer.join();

    std::cout << "\nTelemetry Statistics" << std::endl;
    std::cout << "--------------------" << std::endl;

    std::cout << "Messages processed: "
            << processor.getProcessedCount()
            << std::endl;

    std::cout << "Invalid messages: "
            << processor.getInvalidCount()
            << std::endl;

    std::cout << "Minimum temperature: "
            << processor.getMinTemp()
            << " C" << std::endl;

    std::cout << "Maximum temperature: "
            << processor.getMaxTemp()
            << " C" << std::endl;

    std::cout << "Average temperature: "
            << processor.getAverageTemp()
            << " C" << std::endl;

    const DeviceStatistics& sensorOneStats =
    processor.getDeviceStatistics("sensor-01");

    std::cout << "\nsensor-01 Statistics" << std::endl;
    std::cout << "Messages: "
            << sensorOneStats.processedCount
            << std::endl;

    std::cout << "Minimum temperature: "
            << sensorOneStats.minTemp
            << " C" << std::endl;

    std::cout << "Maximum temperature: "
            << sensorOneStats.maxTemp
            << " C" << std::endl;

    std::cout << "Average temperature: "
            << processor.getDeviceAverageTemp("sensor-01")
            << " C" << std::endl;

    const DeviceStatistics& sensorTwoStats =
    processor.getDeviceStatistics("sensor-02");

    std::cout << "\nsensor-02 Statistics" << std::endl;
    std::cout << "Messages: "
            << sensorTwoStats.processedCount
            << std::endl;

    std::cout << "Minimum temperature: "
            << sensorTwoStats.minTemp
            << " C" << std::endl;

    std::cout << "Maximum temperature: "
            << sensorTwoStats.maxTemp
            << " C" << std::endl;

    std::cout << "Average temperature: "
            << processor.getDeviceAverageTemp("sensor-02")
            << " C" << std::endl;

    const DeviceStatistics& sensorThreeStats =
    processor.getDeviceStatistics("sensor-03");

    std::cout << "\nsensor-03 Statistics" << std::endl;
    std::cout << "Messages: "
            << sensorThreeStats.processedCount
            << std::endl;

    std::cout << "Minimum temperature: "
            << sensorThreeStats.minTemp
            << " C" << std::endl;

    std::cout << "Maximum temperature: "
            << sensorThreeStats.maxTemp
            << " C" << std::endl;

    std::cout << "Average temperature: "
            << processor.getDeviceAverageTemp("sensor-03")
            << " C" << std::endl;

    std::cout << "Telemetry pipeline finished." << std::endl;

    return 0;
}