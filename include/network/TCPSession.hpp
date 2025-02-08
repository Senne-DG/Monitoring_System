#ifndef TCPSESSION_HPP
#define TCPSESSION_HPP

#include <boost/asio.hpp>
#include <memory>
#include "sensor/SensorData.hpp"
#include "circularBuffer/CircularBuffer.hpp"

using boost::asio::ip::tcp;

/**
 * @class TCPSession
 * @brief Manages an individual TCP connection for receiving sensor data.
 *
 * The TCPSession class encapsulates asynchronous operations over a TCP socket.
 * It continuously reads sensor data from the socket and pushes the received data
 * into a shared circular buffer for further processing.
 *
 * Inheriting from std::enable_shared_from_this allows instances of this class to
 * safely create shared_ptr references to themselves, which is particularly useful
 * during asynchronous operations.
 */
class TCPSession : public std::enable_shared_from_this<TCPSession> {
public:
    /**
     * @brief Constructs a new TCPSession object.
     *
     * @param socket A connected TCP socket.
     * @param buffer Reference to a CircularBuffer for SensorData with a capacity of 1024 elements.
     */
    TCPSession(tcp::socket socket,
               CircularBuffer<SensorData, 1024>& buffer);

    /**
     * @brief Starts the asynchronous read loop.
     *
     * Initiates the asynchronous read operations on the socket to receive sensor data.
     */
    void start();

    /**
     * @brief Stops the TCP session.
     *
     * Gracefully closes the socket and terminates any pending asynchronous operations.
     */
    void stop();

private:
    /**
     * @brief Performs an asynchronous read operation.
     *
     * This internal method is used to initiate a non-blocking read from the socket.
     * Once data is received, it is processed and stored in the shared circular buffer.
     */
    void do_read();

    tcp::socket socket_;                                ///< The TCP socket used for communication.
    SensorData data_;                                   ///< Temporary storage for incoming sensor data.
    CircularBuffer<SensorData, 1024>& sensorBuffer_;    ///< Reference to the shared circular buffer for sensor data.
};

#endif // TCPSESSION_HPP
