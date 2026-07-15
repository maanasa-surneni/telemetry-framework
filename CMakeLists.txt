cmake_minimum_required(VERSION 3.20)

project(
    TelemetryFramework
    VERSION 0.1.0
    DESCRIPTION "Real-time Telemetry Processing Framework"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(
    telemetry_core
    src/TelemetryMessage.cpp
    src/SensorSimulator.cpp
    src/TelemetryProcessor.cpp
    src/CsvExporter.cpp
    src/TelemetryQueue.cpp
)

target_include_directories(
    telemetry_core
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(
    telemetry
    src/main.cpp
)

target_link_libraries(
    telemetry
    PRIVATE
    telemetry_core
)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    telemetry_tests
    tests/TelemetryTests.cpp
)

target_link_libraries(
    telemetry_tests
    PRIVATE
    telemetry_core
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(telemetry_tests)
