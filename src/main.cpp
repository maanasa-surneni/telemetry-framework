#include <iostream>
#include "TelemetryMessage.hpp"

int main()
{
    TelemetryMessage message(
        "sensor-01",
        1720560000,
        24.7,
        91
    );

    message.print();

    return 0;

}