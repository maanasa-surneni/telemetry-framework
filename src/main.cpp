#include <iostream>
#include <chrono>
#include <thread>

#include "SensorSimulator.hpp"
#include "TelemetryQueue.hpp"

int main()
{
    SensorSimulator simulator("sensor-01");

    // The queue can store up to 10 messages.
    TelemetryQueue queue(2);

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
            output.print();

            std::this_thread::sleep_for(
                std::chrono::milliseconds(300)
            );
        }

        std::cout << "Consumer finished processing messages." << std::endl;
    });

    producer.join();
    consumer.join();

    std::cout << "Telemetry pipeline finished." << std::endl;

    return 0;
}