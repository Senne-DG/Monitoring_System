#include <iostream>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>
#include <random>
#include <cstring>

#include "sensor/SensorData.hpp"

using boost::asio::ip::tcp;

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " -p <port> -ip <ip_address>" << std::endl;
    std::cerr << "\nOptions:" << std::endl;
    std::cerr << "  -p <port>      Specify the port number of the TCP server to connect to." << std::endl;
    std::cerr << "  -ip <address>  Specify the IP address of the TCP server." << std::endl;
    std::cerr << "  --help         Show this help message and exit." << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        std::string ip_address = "127.0.0.1";
        std::string port;

        for (int i = 1; i < argc; ++i) {
            if (std::string(argv[i]) == "--help" || std::string(argv[i]) == "-h") {
                printUsage(argv[0]);
                return 0;
            } else if (std::strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
                port = argv[++i];
            } else if (std::strcmp(argv[i], "-ip") == 0 && i + 1 < argc) {
                ip_address = argv[++i];
            } else {
                printUsage(argv[0]);
                return 1;
            }
        }

        if (port.empty()) {
            printUsage(argv[0]);
            return 1;
        }

        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(ip_address, port);

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::random_device rd;
        std::mt19937 gen(rd());

        // Generate a random sensor ID between 1 and 1000
        std::uniform_int_distribution<> id_dis(1, 1000);
        int sensor_id = id_dis(gen);

        // Create a uniform distribution for temperatures between 26.6 and 46.6 degrees.
        std::uniform_real_distribution<> dis(26.6, 46.6);

        while (true) {
            SensorData data;
            data.id = sensor_id;
            data.temperature = static_cast<float>(dis(gen));

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
