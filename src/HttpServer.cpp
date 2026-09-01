#include "HttpServer.hpp"
#include "HttpRequestParser.hpp"
#include "HttpResponse.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

HttpServer::HttpServer(uint16_t port)
    : port_(port), server_fd_(-1), is_running_(false) {
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
            std::cout << "Error accepting connection" << std::endl;
            continue;
        }
        handle_client(client_fd);
    }
}

void HttpServer::handle_client(int client_fd) {
    char buffer[4096] = {0};
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        std::cout << "Error reading from socket" << std::endl;
        close(client_fd);
        return;
    }
    buffer[bytes_read] = '\0';

    std::string request = buffer;
    HttpRequest req = HttpRequestParser::parse(request);
    HttpResponse res;
    res.set_header("Content-Type", "text/plain");
    res.set_body(req.path);

    std::string response_str = res.to_string();
    write(client_fd, response_str.c_str(), response_str.size());

    close(client_fd);

}

void HttpServer::stop() {
    is_running_ = false;
    if (server_fd_ >= 0) {
        close(server_fd_);
        server_fd_ = -1;
    }
}