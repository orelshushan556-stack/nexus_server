#include "StaticFileHandler.hpp"

StaticFileHandler::StaticFileHandler(const std::filesystem::path& public_root)
    : public_root_(std::filesystem::weakly_canonical(public_root)) {
}

std::optional<HttpResponse> StaticFileHandler::serve(std::string_view raw_path) const {
    std::string rel_path(raw_path);
    if (rel_path == "/" || rel_path.empty()) {
        rel_path = "index.html";
    }
    if (rel_path.front() == '/') {
        rel_path.erase(0, 1);
    }

    std::filesystem::path target = std::filesystem::weakly_canonical(public_root_ / rel_path);

    if (!is_safe_path(target) || !std::filesystem::is_regular_file(target)) {
        return std::nullopt;
    }

    std::ifstream file(target, std::ios::binary);
    if (!file.is_open()) {
        return std::nullopt;
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());

    HttpResponse res;
    res.set_status(200, "OK");
    res.set_header("Content-Type", std::string(get_mime_type(target)));
    res.set_header("Content-Length", std::to_string(content.size()));
    res.set_body(std::move(content));

    return res;
}

std::string_view StaticFileHandler::get_mime_type(const std::filesystem::path& file_path) {
    static const std::unordered_map<std::string, std::string_view> mime_types = {
        {".html", "text/html"},
        {".htm",  "text/html"},
        {".css",  "text/css"},
        {".js",   "application/javascript"},
        {".wasm", "application/wasm"},
        {".json", "application/json"},
        {".png",  "image/png"},
        {".svg",  "image/svg+xml"},
        {".bin",  "application/octet-stream"}
    };
    auto it = mime_types.find(file_path.extension().string());
    if (it != mime_types.end()) {
        return it->second;
    }
    return "application/octet-stream";

}

bool StaticFileHandler::is_safe_path(const std::filesystem::path& resolved_path) const {
    const auto target = resolved_path.string();
    const auto base = public_root_.string();

    return target.starts_with(base);
}