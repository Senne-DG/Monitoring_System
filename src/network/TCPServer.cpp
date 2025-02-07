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
                // Create a new session.
                auto session = std::make_shared<TCPSession>(std::move(socket), sensorBuffer_);
                // Add the session to the connection manager.
                connectionManager_.add(session);
                // Start the session.
                session->start();
            } else {
                std::cerr << "Accept error: " << ec.message() << std::endl;
            }
            // Continue accepting connections.
            do_accept();
        }
    );
}
