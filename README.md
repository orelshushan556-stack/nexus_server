# Nexus Server 🚀

A lightweight, zero-dependency HTTP/1.1 web server written from scratch in modern **C++20** using POSIX sockets.

## Key Features
- **Zero External Dependencies**: Pure C++ standard library & POSIX networking APIs.
- **Strict HTTP/1.1 Compliance**: Complete request parsing (methods, headers, body) and structured `HttpResponse` serialization.
- **Clean Modular Architecture**: Separation of concerns between socket lifecycle, request parsing, routing, and response generation.
- **Modern C++ Design**: Built with modern C++20 idioms, strict compiler flags (`-Wall -Wextra -Wpedantic`), and CMake build automation.

## Roadmap
- [x] Socket setup & non-blocking connection handling
- [x] `HttpRequestParser` & `HttpResponse` classes
- [ ] Path routing & HTTP status codes (200, 404, 500)
- [ ] Custom Thread Pool for concurrent client connections
- [ ] Static asset serving (HTML/CSS/MIME types)
- [ ] Benchmarking & performance profiling

## Build & Run
```bash
mkdir build && cd build
cmake ..
make
./nexus_server