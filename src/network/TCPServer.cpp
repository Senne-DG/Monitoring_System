#include "network/TCPServer.hpp"
#include "logging/LoggerClient.hpp"
#include <iostream>

TCPServer::TCPServer(boost::asio::io_context& io_context, short port,
                     CircularBuffer<SensorData, 1024>& buffer)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
      sensorBuffer_(buffer)
{
    do_accept();
}

void TCPServer::do_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                TRACE_INFO("A new TCP connections is accepted");

                auto session = std::make_shared<TCPSession>(std::move(socket), sensorBuffer_);
                connectionManager_.add(session);
                session->start();
            } else {
                TRACE_ERROR("Error occured during socket acception: %s", ec.message());
            }
            do_accept();
        }
    );
}
