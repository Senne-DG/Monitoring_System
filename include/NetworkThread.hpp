#ifndef NETWORK_THREAD_HPP
#define NETWORK_THREAD_HPP

#include "sensor/SensorData.hpp"
#include "circularBuffer/CircularBuffer.hpp"

// Starts the network thread, which sets up and runs the TCP server.
// The circular buffer is passed in by reference.
void start_network_thread(CircularBuffer<SensorData, 1024>& sensorBuffer);

#endif // NETWORK_THREAD_HPP
