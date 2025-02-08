#include <thread>
#include <iostream>

#include "NetworkThread.hpp"
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
        std::thread networkThread(start_network_thread, std::ref(sensorBuffer));


        TRACE_INFO("Threads are joining for shutdown...");
        networkThread.join();
    }
    catch (const std::exception& e) {
        TRACE_ERROR("An error occurred while starting up the application, with %s", e.what());
    }
    return 0;
}
