#include <iostream>
#include <chrono>
#include <thread>

#include "SensorSimulator.hpp"
#include "TelemetryQueue.hpp"
#include "TelemetryProcessor.hpp"

int main()
{
    SensorSimulator sensorOne("sensor-01");
    SensorSimulator sensorTwo("sensor-02");
    SensorSimulator sensorThree("sensor-03");

    // The queue can store up to 10 messages.
    TelemetryQueue queue(10);

    TelemetryProcessor processor;

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
            processor.process(output); //every consumed message is passed into this
            output.print();
        }

        std::cout << "Consumer finished processing messages." << std::endl;
    });

    producerOne.join();
    producerTwo.join();
    producerThree.join();

    queue.stop();

    consumer.join();

    std::cout << "\nTelemetry Statistics\n";
    std::cout << "--------------------\n";

    std::cout << "Messages processed: "
            << processor.getProcessedCount()
            << '\n';

    std::cout << "Invalid messages: "
            << processor.getInvalidCount()
            << '\n';

    std::cout << "Minimum temperature: "
            << processor.getMinTemp()
            << " C\n";

    std::cout << "Maximum temperature: "
            << processor.getMaxTemp()
            << " C\n";

    std::cout << "Average temperature: "
            << processor.getAverageTemp()
            << " C\n";

    const DeviceStatistics& sensorOneStats =
    processor.getDeviceStatistics("sensor-01");

    std::cout << "\nsensor-01 Statistics\n";
    std::cout << "Messages: "
            << sensorOneStats.processedCount
            << '\n';

    std::cout << "Minimum temperature: "
            << sensorOneStats.minTemp
            << " C\n";

    std::cout << "Maximum temperature: "
            << sensorOneStats.maxTemp
            << " C\n";

    std::cout << "Average temperature: "
            << processor.getDeviceAverageTemp("sensor-01")
            << " C\n";

    const DeviceStatistics& sensorTwoStats =
    processor.getDeviceStatistics("sensor-02");

    std::cout << "\nsensor-02 Statistics\n";
    std::cout << "Messages: "
            << sensorTwoStats.processedCount
            << '\n';

    std::cout << "Minimum temperature: "
            << sensorTwoStats.minTemp
            << " C\n";

    std::cout << "Maximum temperature: "
            << sensorTwoStats.maxTemp
            << " C\n";

    std::cout << "Average temperature: "
            << processor.getDeviceAverageTemp("sensor-02")
            << " C\n";

    const DeviceStatistics& sensorThreeStats =
    processor.getDeviceStatistics("sensor-03");

    std::cout << "\nsensor-03 Statistics\n";
    std::cout << "Messages: "
            << sensorThreeStats.processedCount
            << '\n';

    std::cout << "Minimum temperature: "
            << sensorThreeStats.minTemp
            << " C\n";

    std::cout << "Maximum temperature: "
            << sensorThreeStats.maxTemp
            << " C\n";

    std::cout << "Average temperature: "
            << processor.getDeviceAverageTemp("sensor-03")
            << " C\n";

    std::cout << "Telemetry pipeline finished." << std::endl;

    return 0;
}