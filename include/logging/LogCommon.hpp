#pragma once

#include <ctime>
#include <cstring>
#include <iomanip>
#include <iostream>

enum LogLevel {
    TRACE_FATAL,  // 0: most severe
    TRACE_ERROR,  // 1
    TRACE_WARN,   // 2
    TRACE_INFO,   // 3
    TRACE_DEBUG   // 4: least severe
};

inline const char* logLevelToString(LogLevel level) {
    switch(level) {
        case TRACE_FATAL: return "[FATAL]  ";
        case TRACE_ERROR: return "[ERROR]  ";
        case TRACE_WARN:  return "[WARN]   ";
        case TRACE_INFO:  return "[INFO]   ";
        case TRACE_DEBUG: return "[DEBUG]  ";
        default:          return "[UNKNOWN]";
    }
}

struct LogMessage {
    LogLevel level;
    std::time_t timestamp;
    char message[256];
};

inline std::ostream& operator<<(std::ostream& os, const LogMessage& logMsg) {
    std::tm* tm_ptr = std::localtime(&logMsg.timestamp);
    if (tm_ptr) {
        // Format the output: YYYY-MM-DD HH:MM:SS [LEVEL] Message
        os << std::put_time(tm_ptr, "%Y-%m-%d %H:%M:%S") << " ";
        os << logLevelToString(logMsg.level) << " ";
        os << logMsg.message;
    } else {
        os << logMsg.timestamp << " " << logLevelToString(logMsg.level) << " " << logMsg.message;
    }
    return os;
}
