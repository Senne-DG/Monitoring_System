#ifndef TCPSESSION_HPP
#define TCPSESSION_HPP

#include <boost/asio.hpp>
#include <memory>
#include "sensor/SensorData.hpp"
#include "circularBuffer/CircularBuffer.hpp"

using boost::asio::ip::tcp;

class TCPSession : public std::enable_shared_from_this<TCPSession> {
public:
    // Constructor accepts a connected socket and a reference to the shared circular buffer.
    TCPSession(tcp::socket socket,
               CircularBuffer<SensorData, 1024>& buffer);

    // Start the asynchronous read loop.
    void start();

    // Stop the session (close the socket).
    void stop();

private:
    // Internal asynchronous read method.
    void do_read();

    tcp::socket socket_;
    SensorData data_;
    CircularBuffer<SensorData, 1024>& sensorBuffer_;
};

#endif // TCPSESSION_HPP
