#pragma once

#include <cstdint>
#include "WorkerThreadPool.hpp"

class HttpServer {
public:
    explicit HttpServer(uint16_t port, size_t num_threads = 4);
    ~HttpServer();

    HttpServer(const HttpServer&) = delete;
    HttpServer& operator=(const HttpServer&) = delete;

    HttpServer(HttpServer&&) noexcept = default;
    HttpServer& operator=(HttpServer&&) noexcept = default;

    void start();
    void stop();

private:
    uint16_t port_;
    int server_fd_{-1};
    bool is_running_{false};
    WorkerThreadPool thread_pool_;

    void setup_socket();
    static void handle_client(int client_fd);
};