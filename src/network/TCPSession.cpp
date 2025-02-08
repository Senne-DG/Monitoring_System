#include <iostream>
#include <boost/asio.hpp>

#include "network/TCPSession.hpp"
#include "logging/LoggerClient.hpp"

TCPSession::TCPSession(tcp::socket socket,
                       CircularBuffer<SensorData, 1024>& buffer)
    : socket_(std::move(socket)),
      sensorBuffer_(buffer)
{
}

void TCPSession::start() {
    TRACE_INFO("TCPSessions started");
    do_read();
}

void TCPSession::stop() {
    TRACE_INFO("TCPSessions stopped");
    boost::system::error_code ec;
    socket_.close(ec);
    if (ec) {
        TRACE_ERROR("Error occured during closing socket: %s", ec.what());
    }
}

void TCPSession::do_read() {
    auto self(shared_from_this());
    boost::asio::async_read(socket_,
        boost::asio::buffer(&data_, sizeof(SensorData)),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                sensorBuffer_.in(data_);
                TRACE_INFO("Received sensor data: ID=%d, Temperature=%f", data_.id, data_.temperature);
                do_read();
            } else {
                TRACE_ERROR("Error during reading: %s", ec.what());
                // TODO:: notify the connection manager here.
                stop();
            }
        }
    );
}
