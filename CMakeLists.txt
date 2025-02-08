cmake_minimum_required(VERSION 3.10)
project(MonitoringSystem VERSION 1.0)

# Set the C++ standard.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Force module mode for Boost.
set(Boost_NO_BOOST_CMAKE ON)

# Find Boost (we require at least Boost.System for Asio).
find_package(Boost REQUIRED COMPONENTS system)

if(Boost_FOUND)
    message(STATUS "Found Boost at: ${Boost_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Boost not found!")
endif()

# Optionally, verify that the Boost.Interprocess header is present.
set(BOOST_INTERPROCESS_HEADER_FOUND OFF)
foreach(dir ${Boost_INCLUDE_DIRS})
    if(EXISTS "${dir}/boost/interprocess/ipc/message_queue.hpp")
        set(BOOST_INTERPROCESS_HEADER_FOUND ON)
    endif()
endforeach()

if(NOT BOOST_INTERPROCESS_HEADER_FOUND)
    message(FATAL_ERROR "Could not find boost/interprocess/ipc/message_queue.hpp in Boost include directories!")
endif()

# Include directories for public headers.
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/utils
    ${PROJECT_SOURCE_DIR}/src/logging
    ${Boost_INCLUDE_DIRS}
)

# -------------------------------------------------------------------
# Main Application executable (monitoring_system)
#
# This target includes:
#   - main.cpp (which spawns the network thread)
#   - NetworkThread.cpp (which sets up the TCP server)
#   - TCP server related sources: TCPServer.cpp, TCPSession.cpp, TCPConnectionManager.cpp
# -------------------------------------------------------------------
add_executable(monitoringSystem
    src/main.cpp
    src/NetworkThread.cpp
    src/DataManagerThread.cpp
    src/network/TCPServer.cpp
    src/network/TCPSession.cpp
    src/network/TCPConnectionManager.cpp
)

target_link_libraries(monitoringSystem ${Boost_LIBRARIES})

# -------------------------------------------------------------------
# Logger Server executable (logger_server)
#
# This target builds your separate logging process.
# It will have access to the logging headers from include/logging.
# -------------------------------------------------------------------
add_executable(loggerServer
    services/LoggerServer.cpp
)

target_link_libraries(loggerServer ${Boost_LIBRARIES})

# -------------------------------------------------------------------
# Sensor Client executable (sensor_client)
#
# This target simulates sensor data by connecting to the TCP server.
# -------------------------------------------------------------------
add_executable(sensorClient
    clients/SensorClient.cpp
)

target_link_libraries(sensorClient ${Boost_LIBRARIES})

# -------------------------------------------------------------------
# Set Log Level Utility executable (set_log_level)
#
# This target builds a small utility that sends a command (via Boost.Interprocess)
# to update the global log level in your running processes.
# The source file is located in src/logging/setLogLevel.cpp.
# -------------------------------------------------------------------
add_executable(set_log_level
    src/logging/setLogLevel.cpp
)

target_link_libraries(set_log_level ${Boost_LIBRARIES})
