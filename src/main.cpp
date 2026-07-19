#include <iostream>
#include <chrono>
#include <thread>

#include "SensorSimulator.hpp"
#include "TelemetryQueue.hpp"
#include "TelemetryProcessor.hpp"

int main()
{
    SensorSimulator simulator("sensor-01");

    // The queue can store up to 10 messages.
    TelemetryQueue queue(2);

    TelemetryProcessor processor;

    constexpr int messageCount{10};

    //lambda capture which means that the thread's lambda can access local variables from main() by reference
    std::thread producer([&] {
        for (int i{0}; i<messageCount; ++i){
            TelemetryMessage message = simulator.generateMessage();

            if (message.isValid()) {
                if (!queue.waitPush(message)) {
                    break;
                }

                std::cout << "Producer queued message " << i+1 << std::endl;
            }

            std::this_thread::sleep_for(
                std::chrono::milliseconds(100)
            );
        }

        queue.stop();
    });

    TelemetryMessage output("placeholder", 0, 0.0, 0);

    std::thread consumer([&] {
        while (queue.waitPop(output)) {
            processor.process(output); //every consumed message is passed into this
            output.print();
        }

        std::cout << "Consumer finished processing messages." << std::endl;
    });

    producer.join();
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

    std::cout << "Telemetry pipeline finished." << std::endl;

    return 0;
}