#include <cassert>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "HttpRequestParser.hpp"
#include "HttpResponse.hpp"
#include "Router.hpp"
#include "StaticFileHandler.hpp"

namespace fs = std::filesystem;

void setup_test_environment() {
    fs::create_directories("public");
    std::ofstream test_file("public/index.html");
    test_file << "<h1>Test Home Page</h1>";
    test_file.close();

    std::ofstream css_file("public/style.css");
    css_file << "body { background: black; }";
    css_file.close();
}

void cleanup_test_environment() {
    fs::remove_all("public");
}

void test_default_file_serve() {
    Router router;
    HttpRequest req;
    req.method = "GET";
    req.path = "/";

    HttpResponse res = router.route(req);
    assert(res.get_status_code() == 200);
    assert(res.get_body() == "<h1>Test Home Page</h1>");
    assert(res.get_headers().at("Content-Type") == "text/html");
}

void test_specific_file_serve() {
    Router router;
    HttpRequest req;
    req.method = "GET";
    req.path = "/style.css";

    HttpResponse res = router.route(req);
    assert(res.get_status_code() == 200);
    assert(res.get_body() == "body { background: black; }");
    assert(res.get_headers().at("Content-Type") == "text/css");
}

void test_path_traversal_attack() {
    Router router;
    HttpRequest req;
    req.method = "GET";
    req.path = "/../../etc/passwd";

    HttpResponse res = router.route(req);
    assert(res.get_status_code() == 404);
}

void test_file_not_found() {
    Router router;
    HttpRequest req;
    req.method = "GET";
    req.path = "/non_existent_file.js";

    HttpResponse res = router.route(req);
    assert(res.get_status_code() == 404);
}

int main() {
    setup_test_environment();

    try {
        test_default_file_serve();
        test_specific_file_serve();
        test_path_traversal_attack();
        test_file_not_found();
        std::cout << "[SUCCESS] All static file tests passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[FAIL] Test exception: " << e.what() << std::endl;
        cleanup_test_environment();
        return 1;
    }

    cleanup_test_environment();
    return 0;
}