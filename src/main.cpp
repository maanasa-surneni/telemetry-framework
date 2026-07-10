#include <iostream>
#include "TelemetryMessage.hpp"

int main()
{
    //from the class TelemetryMessage, it creates an object called message
    TelemetryMessage validMessage(
        "sensor-01",
        1720560000,
        24.7,
        91
    );

    validMessage.print();

    if (validMessage.isValid()){
        std::cout << "Message is valid" << std::endl;
    } else {
        std::cout << "Message is invalid" << std::endl;
    }

    TelemetryMessage invalidMessage(
        "",
        -1,
        500.0,
        120
    );

    if (invalidMessage.isValid()){
        std::cout << "Second message is valid." << std::endl;
    } else {
        std::cout << "Second message is invalid." << std::endl;
    }

    return 0;

}