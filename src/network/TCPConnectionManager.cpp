#include "network/TCPConnectionManager.hpp"
#include "network/TCPSession.hpp"

void TCPConnectionManager::add(std::shared_ptr<TCPSession> session) {
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.insert(session);
}

void TCPConnectionManager::remove(std::shared_ptr<TCPSession> session) {
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.erase(session);
}

void TCPConnectionManager::stop_all() {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& session : sessions_) {
        session->stop();
    }
    sessions_.clear();
}
