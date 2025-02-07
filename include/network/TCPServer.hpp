#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <boost/asio.hpp>
#include "network/TCPSession.hpp"
#include "network/TCPConnectionManager.hpp"
#include "sensor/SensorData.hpp"
#include "circularBuffer/CircularBuffer.hpp"

using boost::asio::ip::tcp;

class TCPServer {
public:
    TCPServer(boost::asio::io_context& io_context, short port,
              CircularBuffer<SensorData, 1024>& buffer);

private:
    // Asynchronously accepts incoming connections.
    void do_accept();

    tcp::acceptor acceptor_;
    CircularBuffer<SensorData, 1024>& sensorBuffer_;
    TCPConnectionManager connectionManager_;
};

#endif // TCPSERVER_HPP
