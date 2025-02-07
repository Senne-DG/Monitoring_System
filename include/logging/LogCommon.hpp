#pragma once

#include <ctime>
#include <cstring>
#include <iomanip>


enum LogLevel {
    TRACE_DEBUG,
    TRACE_INFO,
    TRACE_WARN,
    TRACE_ERROR,
    TRACE_FATAL
};

inline const char* logLevelToString(LogLevel level) {
    switch(level) {
        case TRACE_DEBUG: return "[DEBUG]";
        case TRACE_INFO:  return "[INFO]";
        case TRACE_WARN:  return "[WARN]";
        case TRACE_ERROR: return "[ERROR]";
        case TRACE_FATAL: return "[FATAL]";
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