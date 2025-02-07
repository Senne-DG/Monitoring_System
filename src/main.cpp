// src/main.cpp
#include <thread>
#include <iostream>
#include "NetworkThread.hpp"
#include "circularBuffer/CircularBuffer.hpp"
#include "sensor/SensorData.hpp"

int main() {
    try {
        // Create a circular buffer for sensor data.
        CircularBuffer<SensorData, 1024> sensorBuffer;

        // Start the network thread.
        std::thread networkThread(start_network_thread, std::ref(sensorBuffer));

        // Main thread can now process data from the circular buffer,
        // spawn additional worker threads, etc.
        // For now, we simply join the network thread.
        networkThread.join();
    }
    catch (const std::exception& e) {
        std::cerr << "Main thread exception: " << e.what() << std::endl;
    }
    return 0;
}
