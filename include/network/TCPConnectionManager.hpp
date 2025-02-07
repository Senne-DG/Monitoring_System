#ifndef TCP_CONNECTION_MANAGER_HPP
#define TCP_CONNECTION_MANAGER_HPP

#include <set>
#include <memory>
#include <mutex>

class TCPSession;

class TCPConnectionManager {
public:
    // Add a new session to manage.
    void add(std::shared_ptr<TCPSession> session);

    // Remove a session from management.
    void remove(std::shared_ptr<TCPSession> session);

    // Stop all sessions and clear the list.
    void stop_all();

private:
    std::set<std::shared_ptr<TCPSession>> sessions_;
    std::mutex mutex_;
};

#endif // TCP_CONNECTION_MANAGER_HPP
