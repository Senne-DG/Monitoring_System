#include <boost/asio.hpp>
#include <iostream>
#include "NetworkThread.hpp"
#include "network/TCPServer.hpp"

void start_network_thread(CircularBuffer<SensorData, 1024>& sensorBuffer)
{
    try {
        // Create the Boost.Asio I/O context for networking.
        boost::asio::io_context io_context;

        // Create the TCP server on the desired port (e.g., 12345),
        TCPServer server(io_context, 12345, sensorBuffer);

        // Run the I/O context to process network events.
        io_context.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Network thread exception: " << e.what() << std::endl;
    }
}
