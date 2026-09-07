#include "HttpServer.hpp"
#include "HttpRequestParser.hpp"
#include "HttpResponse.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Router.hpp"

HttpServer::HttpServer(uint16_t port, size_t num_threads)
    : port_(port), server_fd_(-1), is_running_(false), thread_pool_(num_threads) {
}

HttpServer::~HttpServer() {
    stop();
}

void HttpServer::setup_socket() {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0) {
        std::cout << "Error creating socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cout << "Error setting socket options" << std::endl;
        close(server_fd_);
        exit(EXIT_FAILURE);
    }
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port_);
    if (bind(server_fd_, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0) {
        std::cout << "Error binding socket" << std::endl;
        close(server_fd_);
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd_, SOMAXCONN) < 0) {
        std::cout << "Error listening socket" << std::endl;
        close(server_fd_);
        exit(EXIT_FAILURE);
    }
}

void HttpServer::start() {
    setup_socket();
    is_running_ = true;
    while (is_running_) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd_, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
        if (client_fd < 0) {
            if (!is_running_) break;
            std::cout << "Error accepting connection" << std::endl;
            continue;
        }

        thread_pool_.enqueue([client_fd]() {
            handle_client(client_fd);
        });
    }
}

void HttpServer::handle_client(int client_fd) {
    char buffer[4096] = {0};
    int bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) {
        std::cout << "Error reading from socket" << std::endl;
#ifdef _WIN32
        closesocket(client_fd);
#else
        close(client_fd);
#endif
        return;
    }
    buffer[bytes_read] = '\0';

    std::string raw_request = buffer;
    HttpRequest req = HttpRequestParser::parse(raw_request);

    Router router;
    HttpResponse res = router.route(req);

    std::string response_str = res.to_string();
    send(client_fd, response_str.c_str(), static_cast<int>(response_str.size()), 0);

#ifdef _WIN32
    closesocket(client_fd);
#else
    close(client_fd);
#endif
}

void HttpServer::stop() {
    is_running_ = false;
    if (server_fd_ >= 0) {
        close(server_fd_);
        server_fd_ = -1;
    }
}