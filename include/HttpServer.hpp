#pragma once

#include <cstdint>

class HttpServer {
public:
    // Constructor marked explicit to prevent implicit conversions from integer to HttpServer
    explicit HttpServer(uint16_t port);

    // Destructor responsible for RAII cleanup (closing open sockets)
    ~HttpServer();

    // Prevent copying to avoid duplicate file descriptors and double-close bugs
    HttpServer(const HttpServer&) = delete;
    HttpServer& operator=(const HttpServer&) = delete;

    // Allow moving if needed in the future (optional default)
    HttpServer(HttpServer&&) noexcept = default;
    HttpServer& operator=(HttpServer&&) noexcept = default;

    // Starts the main server loop and begins accepting incoming connections
    void start();

    // Gracefully stops the server loop and closes sockets
    void stop();

private:
    uint16_t port_;               // Port number (0-65535, 16-bit unsigned integer)
    int server_fd_{-1};           // Listening socket file descriptor (-1 indicates no open socket)
    bool is_running_{false};      // Execution state flag for the server loop

    // Internal helper functions
    void setup_socket();          // Initializes, binds, and configures the listening socket
    static void handle_client(int client_fd); // Reads client request and sends back a response
};