#ifndef NETWORK_THREAD_HPP
#define NETWORK_THREAD_HPP

#include "sensor/SensorData.hpp"
#include "circularBuffer/CircularBuffer.hpp"

/**
 * @brief Starts the network thread that initializes and runs the TCP server.
 *
 * This function spawns a dedicated network thread responsible for setting up and managing
 * the TCP server. The server listens for incoming connections and processes sensor data,
 * which is communicated via the provided circular buffer.
 *
 * @param sensorBuffer Reference to a CircularBuffer of SensorData with a capacity of 1024 elements.
 */
void start_network_thread(CircularBuffer<SensorData, 1024>& sensorBuffer);


#endif // NETWORK_THREAD_HPP
