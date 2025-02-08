#pragma once

#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdarg>
#include <vector>
#include <cstdio>
#include <string>

#include "LogCommon.hpp"

inline LogLevel g_logThreshold = TRACE_INFO;

// Flag for checking the the MQ is available for logging
inline bool g_loggingAvailable = true;

inline const char* shortenFile(const char* file)
{
    const char* project = "monitoring_system";
    const char* pos = std::strstr(file, project);
    if (pos) {
        pos += std::strlen(project);
        if (*pos == '/' || *pos == '\\') {
            pos++;
        }
        return pos;
    }
    return file;
}

inline std::string string_format(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    va_list args_copy;
    va_copy(args_copy, args);
    const int len = std::vsnprintf(nullptr, 0, fmt, args_copy);
    va_end(args_copy);
    std::vector<char> buffer(len + 1);
    std::vsnprintf(buffer.data(), buffer.size(), fmt, args);
    va_end(args);
    return std::string(buffer.data(), len);
}

inline void logMessage(LogLevel level, const std::string &msg)
{

    if (!g_loggingAvailable) {
        return;
    }

    if (level > g_logThreshold)
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
        g_loggingAvailable = false;
    }
}

#define __SHORT_FILE__ shortenFile(__FILE__)

// TRACE MACROS
#define TRACE_DEBUG(fmt, ...) logMessage(TRACE_DEBUG, string_format("(%s:%d) " fmt, __SHORT_FILE__, __LINE__, ##__VA_ARGS__))
#define TRACE_INFO(fmt, ...)  logMessage(TRACE_INFO,  string_format("(%s:%d) " fmt, __SHORT_FILE__, __LINE__, ##__VA_ARGS__))
#define TRACE_WARN(fmt, ...)  logMessage(TRACE_WARN,  string_format("(%s:%d) " fmt, __SHORT_FILE__, __LINE__, ##__VA_ARGS__))
#define TRACE_ERROR(fmt, ...) logMessage(TRACE_ERROR, string_format("(%s:%d) " fmt, __SHORT_FILE__, __LINE__, ##__VA_ARGS__))
#define TRACE_FATAL(fmt, ...) logMessage(TRACE_FATAL, string_format("(%s:%d) " fmt, __SHORT_FILE__, __LINE__, ##__VA_ARGS__))


