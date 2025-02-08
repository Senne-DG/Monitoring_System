#include "DataManagerThread.hpp"
#include "logging/LoggerClient.hpp"
#include <cmath>
#include <chrono>
#include <thread>

#include "kalmanFilter/KalmanFilter.hpp"

// Global flag to signal the data manager thread to stop.
// std::atomic<bool> g_runDataManager{ true };

// Define a threshold for anomaly detection. Adjust based on your sensor’s expected noise level.
constexpr double ANOMALY_THRESHOLD = 10.0;

using DoubleKalmanFilter = kalman::KalmanFilter<double>;

void start_data_manager_thread(CircularBuffer<SensorData, 1024>& sensorBuffer) {
    TRACE_INFO("Data Manager thread starting up.");

    // TODO: Maybe I can implement different kind of filters... (changeable at runtime??)
    DoubleKalmanFilter kf;

    while (true) {
        if (sensorBuffer.empty()) {
            TRACE_DEBUG("Buffer is empty.");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } else {
            SensorData data = sensorBuffer.out();
            TRACE_DEBUG("Read SensorData from buffer with ID=%d, temperature=%f", data.id, data.temperature);

            kf.update(data.temperature);
            double estimate = kf.getEstimate();
            TRACE_INFO("Updated Kalman filter: sensor temperature = %f, current estimate = %f", data.temperature, estimate);

            // Compare the raw sensor value to the Kalman-filtered estimate.
            if (std::abs(data.temperature - estimate) > ANOMALY_THRESHOLD) {
                TRACE_WARN("Anomaly detected! Raw value: %f, Filtered estimate: %f", data.temperature, estimate);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    TRACE_ERROR("Data Manager thread terminating.");
}
