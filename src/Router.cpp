#include "Router.hpp"

HttpResponse Router::route(const HttpRequest& req) const {
    if (req.path == "/api/status") {
        return handle_api_status();
    }

    std::string path_to_serve = (req.path == "/") ? "/index.html" : req.path;

    auto static_res = file_handler_.serve(path_to_serve);
    if (static_res.has_value()) {
        return std::move(*static_res);
    }

    return handle_not_found();
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