#ifndef TCP_CONNECTION_MANAGER_HPP
#define TCP_CONNECTION_MANAGER_HPP

#include <set>
#include <memory>
#include <mutex>

class TCPSession;

/**
 * @brief Manages multiple TCP sessions.
 *
 * The TCPConnectionManager class is responsible for tracking and managing active TCP sessions.
 * It provides thread-safe operations to add, remove, and stop all sessions. This allows for
 * centralized control over the lifecycle of the sessions, ensuring that they can be stopped
 * gracefully when needed.
 */
class TCPConnectionManager {
public:
    /**
     * @brief Adds a new TCP session to the manager.
     *
     * This method registers a new session so that it can be managed and controlled by the
     * connection manager.
     *
     * @param session A shared pointer to the TCPSession to be added.
     */
    void add(std::shared_ptr<TCPSession> session);

    /**
     * @brief Removes an existing TCP session from the manager.
     *
     * This method deregisters a session from management, typically when the session has ended
     * or is no longer needed.
     *
     * @param session A shared pointer to the TCPSession to be removed.
     */
    void remove(std::shared_ptr<TCPSession> session);

    /**
     * @brief Stops all managed TCP sessions and clears the session list.
     *
     * This method terminates all active TCP sessions managed by this class and clears the internal
     * collection. It is typically used during shutdown to ensure all sessions are closed properly.
     */
    void stop_all();

private:
    std::set<std::shared_ptr<TCPSession>> sessions_;    ///< Set of active TCP sessions.
    std::mutex mutex_;                                  ///< Mutex to synchronize access to the sessions set.
};

#endif // TCP_CONNECTION_MANAGER_HPP
