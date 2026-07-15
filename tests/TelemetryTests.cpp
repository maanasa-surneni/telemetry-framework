#include "TelemetryMessage.hpp"
#include "TelemetryProcessor.hpp"
#include "TelemetryQueue.hpp"

#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>

TEST(TelemetryQueueTest, NewQueueIsEmpty){
    TelemetryQueue queue;

    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0); //checks for equality
}

TEST(TelemetryQueueTest, PushAddsMessage){
    TelemetryQueue queue;

    TelemetryMessage message("sensor-01", 1000, 20.0, 90);

    queue.push(message);

    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.size(), 1);
}

TEST(TelemetryQueueTest, PopReturnsMessage){
    TelemetryQueue queue;

    TelemetryMessage original("sensor-01", 1000, 20.0, 90);

    queue.push(original);

    TelemetryMessage result = queue.pop();

    EXPECT_EQ(result.getDeviceId(), "sensor-01");
    EXPECT_DOUBLE_EQ(result.getTemperature(), 20.0);
    EXPECT_EQ(result.getBatteryPercentage(), 90);
    EXPECT_TRUE(queue.empty());
}

TEST(TelemetryQueueTest, MessagesAreRemovedInFifoOrder){
    TelemetryQueue queue;

    queue.push(
        TelemetryMessage("sensor-01", 1000, 10.0, 90)
    );

    queue.push(
        TelemetryMessage("sensor-02", 1001, 20.0, 80)
    );

    queue.push(
        TelemetryMessage("sensor-03", 1002, 30.0, 70)
    );

    TelemetryMessage first = queue.pop();
    TelemetryMessage second = queue.pop();
    TelemetryMessage third = queue.pop();

    EXPECT_EQ(first.getDeviceId(), "sensor-01");
    EXPECT_EQ(second.getDeviceId(), "sensor-02");
    EXPECT_EQ(third.getDeviceId(), "sensor-03");
}

TEST(TelemetryQueueTest, PopFromEmptyQueueThrowsException){
    TelemetryQueue queue;

    EXPECT_THROW(
        queue.pop(),
        std::runtime_error
    );
}