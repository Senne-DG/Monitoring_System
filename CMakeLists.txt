cmake_minimum_required(VERSION 3.10)
project(MonitoringSystem VERSION 1.0)

# -------------------------------------------------------------------
# Set C++ Standard
# -------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -------------------------------------------------------------------
# Find Required Libraries
# -------------------------------------------------------------------
# Boost (Required for Asio)
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost REQUIRED COMPONENTS system)

if(Boost_FOUND)
    message(STATUS "Found Boost at: ${Boost_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Boost not found!")
endif()

# Python3 (Required for matplotlib-cpp)
find_package(Python3 REQUIRED COMPONENTS Development)

# ncurses (Required for terminal plotting)
find_package(Curses REQUIRED)

# -------------------------------------------------------------------
# Include Directories
# -------------------------------------------------------------------
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/utils
    ${PROJECT_SOURCE_DIR}/src/logging
    ${Boost_INCLUDE_DIRS}
    ${Python3_INCLUDE_DIRS}
    ${CURSES_INCLUDE_DIR}  # Include ncurses
    ${PROJECT_SOURCE_DIR}/thirdParty/matplotlib-cpp
    /home/SXD12011/project/monitoring_system/venv/lib/python3.12/site-packages/numpy/_core/include  # Ensure NumPy support
)

# -------------------------------------------------------------------
# Main Application Executable (monitoringSystem)
# -------------------------------------------------------------------
add_executable(monitoringSystem
    src/main.cpp
    src/NetworkThread.cpp
    src/DataManagerThread.cpp
    src/network/TCPServer.cpp
    src/network/TCPSession.cpp
    src/network/TCPConnectionManager.cpp
    src/plotting/Plotter.cpp
    src/plotting/TerminalPlotter.cpp
)

# Link Required Libraries
target_link_libraries(monitoringSystem
    ${Boost_LIBRARIES}
    ${Python3_LIBRARIES}
    ${CURSES_LIBRARIES}  # Link ncurses for terminal plotting
)

# -------------------------------------------------------------------
# Logger Server Executable (loggerServer)
# -------------------------------------------------------------------
add_executable(loggerServer
    services/LoggerServer.cpp
)

target_link_libraries(loggerServer ${Boost_LIBRARIES})

# -------------------------------------------------------------------
# Sensor Client Executable (sensorClient)
# -------------------------------------------------------------------
add_executable(sensorClient
    clients/SensorClient.cpp
)

target_link_libraries(sensorClient ${Boost_LIBRARIES})

# -------------------------------------------------------------------
# Add Terminal Plotter Executable (Separate Process)
# -------------------------------------------------------------------
add_executable(terminalPlotter
    src/plotting/TerminalPlotter.cpp
    src/plotting/main.cpp
)

target_link_libraries(terminalPlotter
    ${CURSES_LIBRARIES}
)

# -------------------------------------------------------------------
# Set Log Level Utility Executable (setLogLevel)
# -------------------------------------------------------------------
add_executable(setLogLevel
    src/logging/setLogLevel.cpp
)

target_link_libraries(setLogLevel ${Boost_LIBRARIES})

# -------------------------------------------------------------------
# Status Messages
# -------------------------------------------------------------------
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Boost Include Dir: ${Boost_INCLUDE_DIRS}")
message(STATUS "Python3 Include Dir: ${Python3_INCLUDE_DIRS}")
message(STATUS "ncurses Include Dir: ${CURSES_INCLUDE_DIR}")
