#include "network/TCPSession.hpp"
#include <iostream>
#include <boost/asio.hpp>

TCPSession::TCPSession(tcp::socket socket,
                       CircularBuffer<SensorData, 1024>& buffer)
    : socket_(std::move(socket)),
      sensorBuffer_(buffer)
{
}

void TCPSession::start() {
    do_read();
}

void TCPSession::stop() {
    boost::system::error_code ec;
    socket_.close(ec);
    if (ec) {
        std::cerr << "Error closing socket: " << ec.message() << std::endl;
    }
}

void TCPSession::do_read() {
    auto self(shared_from_this());
    boost::asio::async_read(socket_,
        boost::asio::buffer(&data_, sizeof(SensorData)),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                // Push received sensor data into the shared circular buffer.
                sensorBuffer_.in(data_);
                int size = sensorBuffer_.size();
                std::cout << "Received sensor data: ID=" << data_.id
                          << ", Temperature=" << data_.temperature << ", Buffer size=" << size << std::endl;
                do_read();
            } else {
                std::cerr << "Read error: " << ec.message() << std::endl;
                // Optionally, you could notify the connection manager here.
                // For now, simply stop the session.
                stop();
            }
        }
    );
}
