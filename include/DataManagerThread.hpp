#ifndef DATA_MANAGER_THREAD_HPP
#define DATA_MANAGER_THREAD_HPP

#include <atomic>
#include "circularBuffer/CircularBuffer.hpp"
#include "sensor/SensorData.hpp"

// Global flag to control the running state of the data manager thread.
// extern std::atomic<bool> g_runDataManager;

// Function that runs the data manager thread loop.
void start_data_manager_thread(CircularBuffer<SensorData, 1024>& sensorBuffer);

#endif // DATA_MANAGER_THREAD_HPP