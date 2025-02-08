#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <boost/asio.hpp>
#include "network/TCPSession.hpp"
#include "network/TCPConnectionManager.hpp"
#include "sensor/SensorData.hpp"
#include "circularBuffer/CircularBuffer.hpp"

using boost::asio::ip::tcp;

/**
 * @brief Manages incoming TCP connections and sensor data sessions.
 *
 * The TCPServer class is responsible for setting up a TCP acceptor on a specified port,
 * accepting incoming connections asynchronously, and managing the corresponding TCP sessions.
 * It utilizes a shared circular buffer to store sensor data received from connected clients,
 * while the TCPConnectionManager oversees active connections.
 */
class TCPServer {
public:
    /**
     * @brief Constructs a TCPServer object.
     *
     * This constructor initializes the server with the provided I/O context, port, and shared
     * circular buffer. It sets up the acceptor to listen on the specified port and prepares the
     * connection manager to handle incoming TCP sessions.
     *
     * @param io_context The Boost.Asio I/O context used for asynchronous operations.
     * @param port The port number on which the server will listen for incoming connections.
     * @param buffer Reference to a CircularBuffer of SensorData with a capacity of 1024 elements.
     */
    TCPServer(boost::asio::io_context& io_context, short port,
              CircularBuffer<SensorData, 1024>& buffer);

private:
    /**
     * @brief Initiates an asynchronous accept operation.
     *
     * This internal method starts an asynchronous operation to accept new incoming TCP connections.
     * When a connection is accepted, a new TCP session is created and managed by the TCPConnectionManager.
     */
    void do_accept();

    tcp::acceptor acceptor_;                           ///< Accepts incoming TCP connections.
    CircularBuffer<SensorData, 1024>& sensorBuffer_;   ///< Reference to a shared circular buffer for sensor data.
    TCPConnectionManager connectionManager_;           ///< Manages active TCP sessions.
};

#endif // TCPSERVER_HPP
