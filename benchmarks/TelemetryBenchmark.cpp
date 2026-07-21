#include <chrono>
#include <string>
#include <iostream>
#include <thread>
#include <vector>

#include "TelemetryMessage.hpp"
#include "TelemetryProcessor.hpp"
#include "TelemetryQueue.hpp"

int main() {
    constexpr int producerCount{4};
    constexpr int messagesPerProducer{100000};

    TelemetryQueue queue(1000);
    TelemetryProcessor processor;

    std::vector<std::thread> producers;

    const auto start = std::chrono::steady_clock::now();

    TelemetryMessage output("placeholder", 0, 0.0, 0);

    //the consumer removes messages from the queue and sends them to the processor and continues until the queue is stopped and empty
    std::thread consumer([&] {
        while (queue.waitPop(output)) {
            processor.process(output);
        }
    });

    for (int producerIndex{0}; producerIndex < producerCount; ++producerIndex){
        producers.emplace_back([&, producerIndex] {
            const std::string deviceId = "sensor-" + std::to_string(producerIndex+1);
        
            for (int messageIndex{0}; messageIndex < messagesPerProducer; ++messageIndex){
                TelemetryMessage message(
                    deviceId,
                    messageIndex,
                    25.0,
                    90
                );

                if (!queue.waitPush(message)) {
                    return;
                }
            }
        });
    }

    for (std::thread& producer : producers) {
        producer.join();
    }

    queue.stop();
    consumer.join();

    const auto end =
    std::chrono::steady_clock::now();

    const auto elapsed =
        std::chrono::duration_cast<
            std::chrono::milliseconds
        >(end - start);

    const std::size_t expectedCount =
        static_cast<std::size_t>(
            producerCount
        ) *
        static_cast<std::size_t>(
            messagesPerProducer
        );

    const double seconds =
        std::chrono::duration<double>(
            end - start
        ).count();

    const double throughput =
        expectedCount / seconds;

    std::cout << "Telemetry Benchmark\n";
    std::cout << "-------------------\n";

    std::cout << "Producer threads: "
              << producerCount
              << '\n';

    std::cout << "Messages per producer: "
              << messagesPerProducer
              << '\n';

    std::cout << "Expected messages: "
              << expectedCount
              << '\n';

    std::cout << "Processed messages: "
              << processor.getProcessedCount()
              << '\n';

    std::cout << "Elapsed time: "
              << elapsed.count()
              << " ms\n";

    std::cout << "Throughput: "
              << static_cast<std::size_t>(
                     throughput
                 )
              << " messages/second\n";

    if (processor.getProcessedCount()
        != expectedCount) {

        std::cerr
            << "Benchmark failed: message count mismatch\n";

        return 1;
    }

    return 0;
}