#include "Router.hpp"

HttpResponse Router::route(const HttpRequest& req) const {
    if (req.path == "/") {
        return handle_home();
    } else if (req.path == "/api/status") {
        return handle_api_status();
    } else {
        return handle_not_found();
    }
}

HttpResponse Router::handle_home() {
    HttpResponse res;
    res.set_header("Content-Type", "text/html");
    res.set_body("<h1>Hello World</h1>");
    return res;
}

HttpResponse Router::handle_api_status() {
    HttpResponse res;
    res.set_header("Content-Type", "application/json");
    res.set_body(R"({"status":"ok","server":"Nexus"})");
    return res;
}

HttpResponse Router::handle_not_found() {
    HttpResponse res;
    res.set_status(404, "Not Found");
    res.set_header("Content-Type", "text/html");
    res.set_body("<h1>404 Not Found</h1>");
    return res;
}