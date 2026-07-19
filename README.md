# Real-Time Telemetry Processing Framework

A multithreaded telemetry processing framework. This simulates multiple sensor devices, sends telemetry messages through a bounded thread-safe queue, processes statistics, detects alerts, and exports messages to CSV.

## Features

- Multiple simulated sensor devices
- Multithreaded producer-consumer pipeline
- Bounded thread-safe telemetry queue
- Blocking queue using condition variables
- Graceful shutdown
- Overall telemetry statistics
- Per-device statistics
- High-temperature &  low-battery alerts
- CSV export
- Multithreaded stress testing

## Architecture

```text
Sensor Producers
       ↓
TelemetryQueue
       ↓
Consumer Thread
       ↓
TelemetryProcessor
       ├── Statistics
       ├── Alerts
       └── CSV Export