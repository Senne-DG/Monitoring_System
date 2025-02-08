#include <boost/asio.hpp>
#include <iostream>
#include "NetworkThread.hpp"
#include "network/TCPServer.hpp"
#include "logging/LoggerClient.hpp"

void start_network_thread(CircularBuffer<SensorData, 1024>& sensorBuffer)
{
    try {
        boost::asio::io_context io_context;

        TRACE_INFO("Creating TCPServer on port %d", 12345);
        // TODO: Make the port optional with an extra CLI argument (default = 12345)
        TCPServer server(io_context, 12345, sensorBuffer);
        io_context.run();
    }
    catch (const std::exception& e) {
        TRACE_ERROR("Network thread exception occurred: %s", e.what());
    }
}
