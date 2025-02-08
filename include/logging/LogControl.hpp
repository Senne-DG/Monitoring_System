#pragma once

#include <thread>
#include <chrono>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>

#include "LogCommon.hpp"
#include "LoggerClient.hpp"

inline void controlThread() {
    using namespace boost::interprocess;
    try {
        message_queue controlMQ(open_or_create, "LogControlQueue", 10, sizeof(int));

        while (true) {
            int newLevel = -1;
            size_t received_size;
            unsigned int priority;

            if (controlMQ.try_receive(&newLevel, sizeof(newLevel), received_size, priority)) {
                if (received_size == sizeof(newLevel)) {
                    g_logThreshold = static_cast<LogLevel>(newLevel);
                    TRACE_WARN("Log threshold changed to %d", newLevel);
                }
            }
            // Sleep briefly to avoid busy-waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    catch (boost::interprocess::interprocess_exception &ex) {
        std::cerr << "Control thread exception: " << ex.what() << std::endl;
    }
}

inline std::thread startControlThread() {
    return std::thread(controlThread);
}
