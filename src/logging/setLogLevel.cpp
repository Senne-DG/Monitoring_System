#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace boost::interprocess;

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <log_level_number>\n"
              << "Where <log_level_number> is an integer representing the new log level:\n"
              << "  0: TRACE_DEBUG\n"
              << "  1: TRACE_INFO\n"
              << "  2: TRACE_WARN\n"
              << "  3: TRACE_ERROR\n"
              << "  4: TRACE_FATAL\n";
}

int main(int argc, char* argv[]) {
    if (argc != 2 || std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "--help") == 0) {
        printUsage(argv[0]);
        return EXIT_SUCCESS;
    }

    int newLevel = std::atoi(argv[1]);

    try {
        message_queue mq(open_only, "LogControlQueue");
        mq.send(&newLevel, sizeof(newLevel), 0);
        std::cout << "Sent log level change command: " << newLevel << std::endl;
    }
    catch (boost::interprocess::interprocess_exception &ex) {
        std::cerr << "Error sending command: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
