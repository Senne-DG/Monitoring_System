#include <iostream>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>

#include "sensor/SensorData.hpp"

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        // TODO: If TCPServer port is configurable, also this one needs to be configurable with CLI command
        auto endpoints = resolver.resolve("127.0.0.1", "12345");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        while (true) {
            // TODO: Make this data more random
            SensorData data;
            data.id = 42;              // Example sensor id.
            data.temperature = 36.6f;  // Example temperature value.

            // Send the sensor data as binary.
            boost::asio::write(socket, boost::asio::buffer(&data, sizeof(data)));

            std::cout << "Sent sensor data: "
                      << "ID = " << data.id
                      << ", Temperature = " << data.temperature << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception in SensorClient: " << e.what() << std::endl;
    }

    return 0;
}
