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
# This covers include/logging, include/network, include/sensor, etc.
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/utils
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
add_executable(monitoring_system
    src/main.cpp
    src/NetworkThread.cpp
    src/network/TCPServer.cpp
    src/network/TCPSession.cpp
    src/network/TCPConnectionManager.cpp
)

target_link_libraries(monitoring_system ${Boost_LIBRARIES})

# -------------------------------------------------------------------
# Logger Server executable (logger_server)
#
# This target builds your separate logging process.
# It will have access to the logging headers from include/logging.
# -------------------------------------------------------------------
add_executable(logger_server
    services/LoggerServer.cpp
)

target_link_libraries(logger_server ${Boost_LIBRARIES})

# -------------------------------------------------------------------
# Sensor Client executable (sensor_client)
#
# This target simulates sensor data by connecting to the TCP server.
# -------------------------------------------------------------------
add_executable(sensor_client
    clients/SensorClient.cpp
)

target_link_libraries(sensor_client ${Boost_LIBRARIES})
