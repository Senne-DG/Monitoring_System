#include <thread>
#include <iostream>

#include "NetworkThread.hpp"
#include "DataManagerThread.hpp"
#include "circularBuffer/CircularBuffer.hpp"
#include "sensor/SensorData.hpp"
#include "logging/LoggerClient.hpp"
#include "logging/LogControl.hpp"

int main() {
    try {
        TRACE_INFO("Starting controlThread for controlling trace levels");
        std::thread ctrlThread = startControlThread();
        ctrlThread.detach(); // detaching because this thread is present for whole lifetime of the application

        CircularBuffer<SensorData, 1024> sensorBuffer;

        TRACE_INFO("Threads are starting up...");
        // Start the network thread (producer) that writes sensor data to the buffer.
        std::thread networkThread(start_network_thread, std::ref(sensorBuffer));

        // Start the data manager thread (consumer) that reads sensor data, filters it, and checks for anomalies.
        std::thread dataManagerThread(start_data_manager_thread, std::ref(sensorBuffer));


        // TODO: Make something to gracefully stop the application (globalFlag)


        TRACE_INFO("Threads are joining");
        networkThread.join();
        dataManagerThread.join();
    }
    catch (const std::exception& e) {
        TRACE_ERROR("An error occurred while starting up the application, with %s", e.what());
    }
    return 0;
}
