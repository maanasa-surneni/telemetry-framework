#include "TelemetryMessage.hpp"
#include "TelemetryProcessor.hpp"
#include "TelemetryQueue.hpp"
#include "CsvExporter.hpp"
#include "TelemetryAlert.hpp"
#include "TelemetryFilter.hpp"

#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <string>
#include <vector>


TEST(TelemetryQueueTest, NewQueueIsEmpty){
    TelemetryQueue queue(5);

    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0); //checks for equality
}

TEST(TelemetryQueueTest, PushAddsMessage){
    TelemetryQueue queue(5);

    TelemetryMessage message("sensor-01", 1000, 20.0, 90);

    bool pushed = queue.tryPush(message);

    EXPECT_TRUE(pushed);
    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.size(), 1);
}

TEST(TelemetryQueueTest, PopReturnsMessage){
    TelemetryQueue queue(5);

    TelemetryMessage original("sensor-01", 1000, 20.0, 90);

    ASSERT_TRUE(queue.tryPush(original));

    TelemetryMessage result("placeholder", 0, 0.0, 0);

    bool popped = queue.tryPop(result);

    ASSERT_TRUE(popped);
    EXPECT_EQ(result.getDeviceId(), "sensor-01");
    EXPECT_DOUBLE_EQ(result.getTemperature(), 20.0);
    EXPECT_EQ(result.getBatteryPercentage(), 90);
    EXPECT_TRUE(queue.empty());
}

TEST(TelemetryQueueTest, MessagesAreRemovedInFifoOrder){
    TelemetryQueue queue(5);

    ASSERT_TRUE(queue.tryPush(
        TelemetryMessage("sensor-01", 1000, 10.0, 90)
    ));

    ASSERT_TRUE(queue.tryPush(
        TelemetryMessage("sensor-02", 1001, 20.0, 80)
    ));

    ASSERT_TRUE(queue.tryPush(
        TelemetryMessage("sensor-03", 1002, 30.0, 70)
    ));

    TelemetryMessage first("placeholder", 0, 0.0, 0);
    TelemetryMessage second("placeholder", 0, 0.0, 0);
    TelemetryMessage third("placeholder", 0, 0.0, 0);

    ASSERT_TRUE(queue.tryPop(first));
    ASSERT_TRUE(queue.tryPop(second));
    ASSERT_TRUE(queue.tryPop(third));

    EXPECT_EQ(first.getDeviceId(), "sensor-01");
    EXPECT_EQ(second.getDeviceId(), "sensor-02");
    EXPECT_EQ(third.getDeviceId(), "sensor-03");
    EXPECT_TRUE(queue.empty());
}

TEST(TelemetryQueueTest, TryPopFromEmptyQueueReturnsFalse) {
    TelemetryQueue queue(5);

    TelemetryMessage result("placeholder", 0, 0.0, 0);

    EXPECT_FALSE(queue.tryPop(result));
    EXPECT_TRUE(queue.empty());
}

TEST(TelemetryQueueTest, TryPushReturnsFalseWhenQueueIsFull) {
    TelemetryQueue queue(2);

    EXPECT_TRUE(queue.tryPush(
        TelemetryMessage("sensor-01", 1000, 10.0, 90)
    ));

    EXPECT_TRUE(queue.tryPush(
        TelemetryMessage("sensor-02", 1001, 20.0, 80)
    ));

    EXPECT_FALSE(queue.tryPush(
        TelemetryMessage("sensor-03", 1002, 30.0, 70)
    ));

    EXPECT_EQ(queue.size(), 2);
}

TEST(TelemetryQueueTest, WaitPopReceivesMessageFromProducer) {
    TelemetryQueue queue(5);

    TelemetryMessage result("placeholder", 0, 0.0, 0);

    std::thread producer([&queue] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        queue.waitPush(
            TelemetryMessage("sensor-01", 1000, 25.0, 90)
        );
    });

    bool popped = queue.waitPop(result);

    producer.join();

    ASSERT_TRUE(popped);
    EXPECT_EQ(result.getDeviceId(), "sensor-01");
    EXPECT_DOUBLE_EQ(result.getTemperature(), 25.0);
    EXPECT_EQ(result.getBatteryPercentage(), 90);
}

TEST(TelemetryQueueTest, StopWakesWaitingConsumer) {
    TelemetryQueue queue(5);

    TelemetryMessage result("placeholder", 0, 0.0, 0);
    bool popped = true;

    std::thread consumer([&] {
        popped = queue.waitPop(result);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    queue.stop();
    consumer.join();

    EXPECT_FALSE(popped);
}

TEST(TelemetryProcessorTest, NewProcessorHasZeroCounts) {
    TelemetryProcessor processor;

    EXPECT_EQ(processor.getProcessedCount(), 0);
    EXPECT_EQ(processor.getInvalidCount(), 0);
    EXPECT_DOUBLE_EQ(processor.getAverageTemp(), 0.0);
}

TEST(TelemetryProcessorTest, ProcessesValidMessage)
{
    TelemetryProcessor processor;

    TelemetryMessage message(
        "sensor-01",
        1000,
        25.0,
        90
    );

    processor.process(message);

    EXPECT_EQ(processor.getProcessedCount(), 1);
    EXPECT_EQ(processor.getInvalidCount(), 0);

    EXPECT_DOUBLE_EQ(
        processor.getAverageTemp(),
        25.0
    );

    EXPECT_DOUBLE_EQ(
        processor.getMinTemp(),
        25.0
    );

    EXPECT_DOUBLE_EQ(
        processor.getMaxTemp(),
        25.0
    );
}

TEST(TelemetryProcessorTest, CalculatesTemperatureStatistics)
{
    TelemetryProcessor processor;

    processor.process(
        TelemetryMessage("sensor-01", 1000, 20.0, 90)
    );

    processor.process(
        TelemetryMessage("sensor-01", 1001, 30.0, 89)
    );

    processor.process(
        TelemetryMessage("sensor-01", 1002, 25.0, 88)
    );

    EXPECT_EQ(processor.getProcessedCount(), 3);

    EXPECT_DOUBLE_EQ(
        processor.getMinTemp(),
        20.0
    );

    EXPECT_DOUBLE_EQ(
        processor.getMaxTemp(),
        30.0
    );

    EXPECT_DOUBLE_EQ(
        processor.getAverageTemp(),
        25.0
    );
}

TEST(TelemetryProcessorTest, CountsInvalidMessage)
{
    TelemetryProcessor processor;

    TelemetryMessage invalidMessage(
        "sensor-01",
        1000,
        25.0,
        -1
    );

    processor.process(invalidMessage);

    EXPECT_EQ(processor.getProcessedCount(), 0);
    EXPECT_EQ(processor.getInvalidCount(), 1);
}

TEST(TelemetryProcessorTest, TracksStatisticsForOneDevice)
{
    TelemetryProcessor processor;

    processor.process(
        TelemetryMessage("sensor-01", 1000, 20.0, 90)
    );

    processor.process(
        TelemetryMessage("sensor-01", 1001, 30.0, 89)
    );

    EXPECT_TRUE(processor.hasDevice("sensor-01"));

    const DeviceStatistics& statistics =
        processor.getDeviceStatistics("sensor-01");

    EXPECT_EQ(statistics.processedCount, 2);
    EXPECT_DOUBLE_EQ(statistics.minTemp, 20.0);
    EXPECT_DOUBLE_EQ(statistics.maxTemp, 30.0);

    EXPECT_DOUBLE_EQ(
        processor.getDeviceAverageTemp("sensor-01"),
        25.0
    );
}

TEST(TelemetryProcessorTest, TracksDevicesSeparately)
{
    TelemetryProcessor processor;

    processor.process(
        TelemetryMessage("sensor-01", 1000, 20.0, 90)
    );

    processor.process(
        TelemetryMessage("sensor-01", 1001, 30.0, 89)
    );

    processor.process(
        TelemetryMessage("sensor-02", 1002, 40.0, 88)
    );

    EXPECT_TRUE(processor.hasDevice("sensor-01"));
    EXPECT_TRUE(processor.hasDevice("sensor-02"));
    EXPECT_FALSE(processor.hasDevice("sensor-03"));

    const DeviceStatistics& sensorOne =
        processor.getDeviceStatistics("sensor-01");

    const DeviceStatistics& sensorTwo =
        processor.getDeviceStatistics("sensor-02");

    EXPECT_EQ(sensorOne.processedCount, 2);
    EXPECT_EQ(sensorTwo.processedCount, 1);

    EXPECT_DOUBLE_EQ(
        processor.getDeviceAverageTemp("sensor-01"),
        25.0
    );

    EXPECT_DOUBLE_EQ(
        processor.getDeviceAverageTemp("sensor-02"),
        40.0
    );

    EXPECT_EQ(processor.getProcessedCount(), 3);
}

TEST(CsvExporterTest, WritesMessageToFile) {
    const std::string filename = "test_telemetry.csv";

    {
        CsvExporter exporter(filename);
        ASSERT_TRUE(exporter.isOpen());

        TelemetryMessage message(
            "sensor-01",
            1000,
            25.5,
            90
        );

        exporter.write(message);
    }

    std::ifstream file(filename);

    ASSERT_TRUE(file.is_open());

    std::string header;
    std::string row;

    std::getline(file, header);
    std::getline(file, row);

    EXPECT_EQ(header, "device_id,timestamp,temperature,battery_percentage");

    EXPECT_EQ(row, "sensor-01,1000,25.5,90");

    std::remove(filename.c_str());
}

TEST(TelemetryAlertTest, DetectsHighTemp) {
    TelemetryAlert alertSystem(28.0, 20);

    TelemetryMessage message(
        "sensor-01",
        1000,
        30.0,
        90
    );

    EXPECT_TRUE(alertSystem.hasHighTemp(message));

    EXPECT_FALSE(alertSystem.hasLowBattery(message));
}

TEST(TelemetryAlertTest, DetectsLowBattery) {
    TelemetryAlert alertSystem(28.0, 20);

    TelemetryMessage message(
        "sensor-01",
        1000,
        25.0,
        10
    );

    EXPECT_FALSE(
        alertSystem.hasHighTemp(message)
    );

    EXPECT_TRUE(
        alertSystem.hasLowBattery(message)
    );
}

TEST(TelemetryAlertTest, CreatesCombinedAlert) {
    TelemetryAlert alertSystem(28.0, 20);

    TelemetryMessage message(
        "sensor-01",
        1000,
        30.0,
        10
    );

    const std::string alert =
        alertSystem.createAlertMessage(message);

    EXPECT_FALSE(alert.empty());
}

TEST(TelemetryPipelineTest, ProcessesLargeNumberOfMessages)
{
    TelemetryQueue queue(100);
    TelemetryProcessor processor;

    constexpr int producerCount{4};
    constexpr int messagesPerProducer{10000};

    const auto start = std::chrono::steady_clock::now();

    //stores multiple producer threads in one container
    std::vector<std::thread> producers;

    TelemetryMessage output(
        "placeholder",
        0,
        0.0,
        0
    );

    std::thread consumer([&] {
        while (queue.waitPop(output)) {
            processor.process(output);
        }
    });

    for (int producerIndex{0};
         producerIndex < producerCount;
         ++producerIndex) {

        //creates a new thread directly inside the vector (each loop iteration creates one producer)
        producers.emplace_back([&, producerIndex] {
            const std::string deviceId =
                "sensor-" + std::to_string(producerIndex);

            for (int i{0};
                 i < messagesPerProducer;
                 ++i) {

                TelemetryMessage message(
                    deviceId,
                    i,
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

    const auto end = std::chrono::steady_clock::now();

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start
    );

    const std::size_t expectedCount =
        producerCount * messagesPerProducer;

    std::cout << "Processed "
              << expectedCount
              << " message in "
              << elapsed.count()
              << " ms" << std::endl;
    
    if (elapsed.count() > 0) {
        const double seconds = elapsed.count() / 1000.0;
        const double throughput = expectedCount / seconds;

        std::cout << "Throughput: "
                  << throughput
                  << " messages/second" << std::endl;
    }

    EXPECT_EQ(
        processor.getProcessedCount(),
        expectedCount
    );
}

TEST(TelemetryFilterTest, AcceptsValidMessage)
{
    TelemetryFilter filter(-40.0, 85.0);

    TelemetryMessage message(
        "sensor-01",
        1000,
        25.0,
        80
    );

    EXPECT_TRUE(filter.accepts(message));
}

TEST(TelemetryFilterTest, RejectsHighTemperature)
{
    TelemetryFilter filter(-40.0, 85.0);

    TelemetryMessage message(
        "sensor-01",
        1000,
        100.0,
        80
    );

    EXPECT_FALSE(filter.accepts(message));
}

TEST(TelemetryFilterTest, RejectsInvalidBattery)
{
    TelemetryFilter filter(-40.0, 85.0);

    TelemetryMessage message(
        "sensor-01",
        1000,
        25.0,
        120
    );

    EXPECT_FALSE(filter.accepts(message));
}

TEST(TelemetryFilterTest, RejectsInvalidTimestamp)
{
    TelemetryFilter filter(-40.0, 85.0);

    TelemetryMessage message(
        "sensor-01",
        0,
        25.0,
        80
    );

    EXPECT_FALSE(filter.accepts(message));
}