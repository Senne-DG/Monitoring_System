#pragma once

#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include "LogCommon.hpp"

inline LogLevel g_logThreshold = TRACE_DEBUG;

inline void logMessage(LogLevel level, const std::string &msg) {
    if (level < g_logThreshold)
        return;

    try {
        boost::interprocess::message_queue mq(
            boost::interprocess::open_only, "LogQueue");

        LogMessage logMsg;
        logMsg.level = level;
        logMsg.timestamp = std::time(nullptr);
        std::strncpy(logMsg.message, msg.c_str(), sizeof(logMsg.message));
        logMsg.message[sizeof(logMsg.message)-1] = '\0';

        // print the message on the screen (stdout)
        std::cout << logMsg << std::endl;

        // print the message in the log file
        mq.send(&logMsg, sizeof(LogMessage), 0);
    }
    catch(boost::interprocess::interprocess_exception &ex) {
        std::cerr << "Logging failed: " << ex.what() << std::endl;
    }
}

// Convenience macros.
#define TRACE_DEBUG(msg) logMessage(TRACE_DEBUG, msg)
#define TRACE_INFO(msg)  logMessage(TRACE_INFO, msg)
#define TRACE_WARN(msg)  logMessage(TRACE_WARN, msg)
#define TRACE_ERROR(msg) logMessage(TRACE_ERROR, msg)
#define TRACE_FATAL(msg) logMessage(TRACE_FATAL, msg)
