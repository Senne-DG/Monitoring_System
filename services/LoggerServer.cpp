// LoggerServer.cpp
#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "logging/LogCommon.hpp"

using namespace boost::interprocess;

std::string formatTimestamp(std::time_t timestamp) {
    std::tm tm = *std::localtime(&timestamp);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

int main() {
    message_queue::remove("LogQueue");

    try {
        // Create a message queue that can store up to 100 messages.
        message_queue mq(create_only, "LogQueue", 100, sizeof(LogMessage));

        std::ofstream logfile("application.log", std::ios::app);
        if (!logfile.is_open()) {
            std::cerr << "Unable to open log file" << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << "Logging service started. Waiting for messages..." << std::endl;

        while (true) {
            LogMessage logMsg;
            size_t received_size;
            unsigned int priority;

            // Block until a message is received.
            mq.receive(&logMsg, sizeof(LogMessage), received_size, priority);

            std::string timeStr = formatTimestamp(logMsg.timestamp);
            logfile << timeStr << " "
                    << logLevelToString(logMsg.level)
                    << ": " << logMsg.message << std::endl;
            logfile.flush();
        }
    }
    catch(interprocess_exception &ex) {
        std::cerr << "Logging service exception: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
