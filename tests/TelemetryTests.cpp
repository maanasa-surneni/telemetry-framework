#include "TelemetryMessage.hpp"
#include "TelemetryProcessor.hpp"
#include "TelemetryQueue.hpp"

#include <gtest/gtest.h>
#include <chrono>
#include <thread>


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