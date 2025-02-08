#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <csignal>
#include <atomic>
#include "logging/LogCommon.hpp"

using namespace boost::interprocess;

std::atomic<bool> g_shutdown{false};

void signal_handler(int signal) {
    if (signal == SIGINT) {
        g_shutdown = true;
        std::cout << "\nSIGINT received. Initiating shutdown..." << std::endl;
    }
}

std::string formatTimestamp(std::time_t timestamp) {
    std::tm tm = *std::localtime(&timestamp);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

int main() {
    std::signal(SIGINT, signal_handler);
    message_queue::remove("LogQueue");

    try {
        message_queue mq(create_only, "LogQueue", 100, sizeof(LogMessage));

        std::ofstream logfile("application.log", std::ios::app);
        if (!logfile.is_open()) {
            std::cerr << "Unable to open log file" << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << "Logging service started. Waiting for messages..." << std::endl;

        LogMessage logMsg;
        size_t received_size;
        unsigned int priority;

        while (!g_shutdown.load()) {
            // Set a timeout for the timed_receive (e.g., 100 milliseconds)
            boost::posix_time::ptime timeout =
                boost::posix_time::microsec_clock::universal_time() +
                boost::posix_time::milliseconds(100);

            // Try to receive a message with a timeout.
            bool messageReceived = mq.timed_receive(&logMsg, sizeof(LogMessage),
                                                    received_size, priority, timeout);

            if (messageReceived) {
                constexpr const char* SHUTDOWN = "shutdown";
                if (std::string(logMsg.message) == SHUTDOWN) {
                    std::cout << "Shutdown command received. Exiting logging service." << std::endl;
                    break;
                }

                // Format and write the log message.
                std::string timeStr = formatTimestamp(logMsg.timestamp);
                logfile << timeStr << " " << logLevelToString(logMsg.level)
                        << ": " << logMsg.message << std::endl;
                logfile.flush();
            }
        }

        std::cout << "Logging service shutting down." << std::endl;
    }
    catch (interprocess_exception &ex) {
        std::cerr << "Logging service exception: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Remove the message queue when done.
    message_queue::remove("LogQueue");
    return EXIT_SUCCESS;
}
