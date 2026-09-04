# Nexus Server 🚀

A lightweight, zero-dependency HTTP/1.1 web server written from scratch in modern **C++20** using POSIX sockets[cite: 1].

## Architecture & Key Features
- **Zero External Dependencies**: Pure C++ standard library & POSIX networking APIs[cite: 1].
- **Core HTTP Server Engine (`HttpServer`)**: Encapsulates socket lifecycle, non-blocking connection dispatch, and request routing integration[cite: 1].
- **Worker Thread Pool (`WorkerThreadPool`)**: High-performance multi-threaded task processing backed by a thread-safe queue, `std::mutex`, and `std::condition_variable` with zero CPU spin-waiting[cite: 1].
- **Graceful Shutdown**: Coordinated teardown via RAII and thread joining to eliminate resource leaks and race conditions[cite: 1].
- **Strict HTTP/1.1 Compliance**: Complete request parsing (`HttpRequestParser`) and structured response serialization (`HttpResponse`)[cite: 1].
- **Declarative Path Routing (`Router`)**: Clean path-to-handler dispatching supporting modular endpoints (`/`, `/api/status`) and standard HTTP status handling (200 OK, 404 Not Found)[cite: 1].
- **Automated Testing Suite**: Dedicated unit and integration tests covering core server flows (`test_server_basic`)[cite: 1].
- **Modern C++ Design**: Built with C++20 idioms, strict compiler flags (`-Wall -Wextra -Wpedantic`), and CMake build automation[cite: 1].

## Concurrency Model
The server employs a worker thread pool model to decouple client connection handling from CPU execution[cite: 1]:
* **Task Ingestion (`enqueue`)**: Client requests are wrapped as `std::function<void()>` and enqueued into a synchronized FIFO task queue[cite: 1].
* **Greedy Worker Loop (`worker_loop`)**: Worker threads sleep on an `std::condition_variable` when idle and wake on demand to pull tasks with automatic load distribution across available CPU cores[cite: 1].
* **Safe Teardown (`~WorkerThreadPool`)**: Broadcasting termination flags, signaling all active threads, and executing `join()` ensures all in-flight tasks finish before destruction[cite: 1].

## Roadmap
- [x] Socket setup & non-blocking connection handling (`HttpServer`)[cite: 1]
- [x] `HttpRequestParser` & `HttpResponse` serialization[cite: 1]
- [x] Declarative path routing & HTTP status codes (`Router`)[cite: 1]
- [x] Custom Thread Pool for concurrent client connections (`WorkerThreadPool`)[cite: 1]
- [x] Automated test suite (`test_server_basic`)[cite: 1]
- [ ] Static asset serving (HTML/CSS/MIME types)[cite: 1]
- [ ] Benchmarking & performance profiling[cite: 1]

## Build & Run
```bash
mkdir -p build && cd build
cmake ..
cmake --build .
./nexus_server