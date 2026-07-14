#include "TelemetryMessage.hpp"
#include "TelemetryProcessor.hpp"

#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>

TEST(TelemetryMessageTest, ValidMessagePassesValidation) {
    TelemetryMessage message(
        "sensor-01",
        1720560000,
        24.7,
        91
    );

    EXPECT_TRUE(message.isValid());
}

TEST(TelemetryMessageTest, EmptyDeviceIdFailsValidation) {
    TelemetryMessage message(
        "",
        1720560000,
        24.7,
        91
    );

    EXPECT_FALSE(message.isValid());
}

TEST(TelemetryMessageTest, BatteryAboveOneHundredFails) {
    TelemetryMessage message(
        "sensor-01",
        1720560000,
        24.7,
        101
    );

    EXPECT_FALSE(message.isValid());
}

TEST(TelemetryMessageTest, NegativeBatteryFailsValidation) {
    TelemetryMessage message(
        "sensor-01",
        1720560000,
        24.7,
        -1
    );

    EXPECT_FALSE(message.isValid());
}

TEST(TelemetryMessageTest, NegativeTimestampFailsValidation) {
    TelemetryMessage message(
        "sensor-01",
        -1,
        24.7,
        91
    );

    EXPECT_FALSE(message.isValid());
}

TEST(TelemetryProcessorTest, CalculatesAverageTemperature)
{
    std::vector<TelemetryMessage> messages{
        TelemetryMessage("sensor-01", 1000, 10.0, 90),
        TelemetryMessage("sensor-01", 1001, 20.0, 80),
        TelemetryMessage("sensor-01", 1002, 30.0, 70)
    };

    TelemetryProcessor processor;

    EXPECT_DOUBLE_EQ(
        processor.calculateAverageTemperature(messages),
        20.0
    );
}

TEST(TelemetryProcessorTest, FindsMinimumTemperature)
{
    std::vector<TelemetryMessage> messages{
        TelemetryMessage("sensor-01", 1000, 14.0, 90),
        TelemetryMessage("sensor-01", 1001, 8.0, 80),
        TelemetryMessage("sensor-01", 1002, 21.0, 70)
    };

    TelemetryProcessor processor;

    EXPECT_DOUBLE_EQ(
        processor.findMinimumTemperature(messages),
        8.0
    );
}

TEST(TelemetryProcessorTest, FindsMaximumTemperature)
{
    std::vector<TelemetryMessage> messages{
        TelemetryMessage("sensor-01", 1000, 14.0, 90),
        TelemetryMessage("sensor-01", 1001, 8.0, 80),
        TelemetryMessage("sensor-01", 1002, 21.0, 70)
    };

    TelemetryProcessor processor;

    EXPECT_DOUBLE_EQ(
        processor.findMaximumTemperature(messages),
        21.0
    );
}

TEST(TelemetryProcessorTest, CalculatesAverageBattery)
{
    std::vector<TelemetryMessage> messages{
        TelemetryMessage("sensor-01", 1000, 20.0, 90),
        TelemetryMessage("sensor-01", 1001, 21.0, 80),
        TelemetryMessage("sensor-01", 1002, 22.0, 70)
    };

    TelemetryProcessor processor;

    EXPECT_DOUBLE_EQ(
        processor.calculateAverageBattery(messages),
        80.0
    );
}

TEST(TelemetryProcessorTest, EmptyMessageListThrowsException)
{
    const std::vector<TelemetryMessage> messages;
    TelemetryProcessor processor;

    EXPECT_THROW(
        processor.calculateAverageTemperature(messages),
        std::invalid_argument
    );
}
