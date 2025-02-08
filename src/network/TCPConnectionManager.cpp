#include "network/TCPConnectionManager.hpp"
#include "network/TCPSession.hpp"
#include "logging/LoggerClient.hpp"

void TCPConnectionManager::add(std::shared_ptr<TCPSession> session) {
    TRACE_DEBUG("TCPConnectionManager received an add request");
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.insert(session);
}

void TCPConnectionManager::remove(std::shared_ptr<TCPSession> session) {
    TRACE_DEBUG("TCPConnectionManager received a remove request");
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.erase(session);
}

void TCPConnectionManager::stop_all() {
    TRACE_DEBUG("TCPConnectionManager received a stop_all request");
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& session : sessions_) {
        session->stop();
    }
    sessions_.clear();
}
