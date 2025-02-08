#include <boost/asio.hpp>
#include <iostream>
#include <cstring>
#include "NetworkThread.hpp"
#include "network/TCPServer.hpp"
#include "logging/LoggerClient.hpp"

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " -p <port>\n";
    std::cerr << "\nOptions:\n";
    std::cerr << "  -p <port>    Specify the port number for the TCP server (default: 12345).\n";
    std::cerr << "  --help       Show this help message and exit.\n";
}

void start_network_thread(int argc, char* argv[], CircularBuffer<SensorData, 1024>& sensorBuffer) {
    try {
        std::string port = "12345";

        for (int i = 1; i < argc; ++i) {
            if (std::string(argv[i]) == "--help" || std::string(argv[i]) == "-h") {
                printUsage(argv[0]);
                return;
            } else if (std::strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
                port = argv[++i];
            } else {
                printUsage(argv[0]);
                return;
            }
        }

        boost::asio::io_context io_context;
        TRACE_INFO("NetworkThread is starting up");
        TRACE_INFO("Creating TCPServer on port %s", port.c_str());

        TCPServer server(io_context, std::stoi(port), sensorBuffer);
        io_context.run();
    }
    catch (const std::exception& e) {
        TRACE_ERROR("Network thread exception occurred: %s", e.what());
    }
}
