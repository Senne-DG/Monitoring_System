#include "DataManagerThread.hpp"
#include "logging/LoggerClient.hpp"
#include <cmath>
#include <chrono>
#include <thread>
#include <vector>

#include "kalmanFilter/KalmanFilter.hpp"
#include "plotting/Plotter.hpp"

// Define a threshold for anomaly detection.
constexpr double ANOMALY_THRESHOLD = 10.0;

using DoubleKalmanFilter = kalman::KalmanFilter<double>;

void start_data_manager_thread(CircularBuffer<SensorData, 1024>& sensorBuffer) {
    TRACE_INFO("Data Manager thread starting up.");

    DoubleKalmanFilter kf;

    std::vector<double> time_series;
    std::vector<double> temp_values;
    std::vector<double> kalman_values;

    int time_counter = 0;

    while (true) {
        if (sensorBuffer.empty()) {
            TRACE_DEBUG("Buffer is empty.");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }

        SensorData data = sensorBuffer.out();
        TRACE_DEBUG("Read SensorData from buffer with ID=%d, temperature=%f", data.id, data.temperature);

        kf.update(data.temperature);
        double estimate = kf.getEstimate();
        TRACE_INFO("Updated Kalman filter: sensor temperature = %f, current estimate = %f", data.temperature, estimate);

        bool is_anomaly = std::abs(data.temperature - estimate) > ANOMALY_THRESHOLD;
        if (is_anomaly) {
            TRACE_WARN("Anomaly detected! Raw value: %f, Filtered estimate: %f", data.temperature, estimate);
            // TODO: Make sure anomalies are shown on the plotter
        }

        // Store data for plotting
        time_series.push_back(time_counter++);
        temp_values.push_back(data.temperature);
        kalman_values.push_back(estimate);

        // Keep only the latest 100 data points
        if (time_series.size() > 100) {
            time_series.erase(time_series.begin());
            temp_values.erase(temp_values.begin());
            kalman_values.erase(kalman_values.begin());
        }

        Plotter::updatePlot(time_series, temp_values, kalman_values, {}, {});

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    TRACE_ERROR("Data Manager thread terminating.");
}
