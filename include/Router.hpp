#pragma once

#include "HttpRequestParser.hpp"
#include "HttpResponse.hpp"

class Router {
public:
    [[nodiscard]] HttpResponse route(const HttpRequest& req) const;

private:
    [[nodiscard]] static HttpResponse handle_home();
    [[nodiscard]] static HttpResponse handle_api_status();
    [[nodiscard]] static HttpResponse handle_not_found();
};