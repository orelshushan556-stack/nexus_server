#include "HttpServer.hpp"
#include <iostream>

int main() {
    std::cout << "[TEST 1] Starting basic server test on port 8080..." << std::endl;
    HttpServer server(8080);
    server.start();
    return 0;
}
