#include <iostream>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>
#include "sensor/SensorData.hpp"

using boost::asio::ip::tcp;

int main() {
    try {
        // Create the I/O context.
        boost::asio::io_context io_context;

        // Resolve the server address and port.
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("127.0.0.1", "12345");

        // Create a socket and connect to the server.
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        // Loop indefinitely to send data every 5 seconds.
        while (true) {
            // Prepare sample sensor data.
            SensorData data;
            data.id = 42;              // Example sensor id.
            data.temperature = 36.6f;  // Example temperature value.

            // Send the sensor data as binary.
            boost::asio::write(socket, boost::asio::buffer(&data, sizeof(data)));

            std::cout << "Sent sensor data: "
                      << "ID = " << data.id
                      << ", Temperature = " << data.temperature << std::endl;

            // Sleep for 5 seconds before sending again.
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception in SensorClient: " << e.what() << std::endl;
    }

    return 0;
}
