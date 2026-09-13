# Nexus Server 🚀

A lightweight, high-performance HTTP/1.1 web server built from scratch in **C++20** with zero external dependencies. The architecture leverages POSIX Sockets for networking, a fully integrated multi-threaded `WorkerThreadPool` for concurrent execution, and a hardened static file serving engine.

> 📄 **Technical Architecture & Benchmarking Whitepaper:**  
> For an exhaustive, deep-dive breakdown of socket lifecycles, RAII concurrency models, and load-test benchmarks, read the complete whitepaper:  
> **[English Version (21 Pages, PDF)](./docs/Nexus_Server_Architecture_Whitepaper_EN.pdf)** | **[Hebrew Version (23 Pages, PDF)](./docs/Nexus_Server_Architecture_Whitepaper_HE.pdf)**

---

## Demo / Preview 🎬
Here is a live demonstration of the frontend (MNIST application) running smoothly on the custom C++20 server backend:

![Nexus Server Demo](assets/test1.gif)

---

## Project Status: Production Ready ✅
* **Core Network Engine**: `HttpServer` utilizing POSIX Sockets, `SO_REUSEADDR` optimization, and cross-platform socket handling (`_WIN32`/POSIX).
* **Concurrency Engine**: Multi-threaded `WorkerThreadPool` protecting the task queue via `std::mutex`, `std::condition_variable`, and RAII locking with safe graceful shutdown.
* **Protocol & Parsing**: Robust HTTP/1.1 request parsing (`HttpRequestParser`) and clean response serialization (`HttpResponse`).
* **Routing & Static Delivery**: Declarative `Router` with automatic root-to-index mapping (`/` -> `/index.html`) and `StaticFileHandler` featuring absolute path isolation, strict MIME-type mapping, and optimized binary file reading (`std::ios::ate`).
* **Security**: Hardened **Path Traversal mitigation** using `std::filesystem::relative` to completely prevent directory traversal attacks (blocking `..` and absolute paths).
* **Load Testing & Performance**: Rigorously benchmarked and verified using **Grafana k6** under high concurrency stress.

---

## Architecture & Engineering Core Concepts

### 1. Networking & POSIX Sockets
* **File Descriptors (fd)**: Treats network connections as standard I/O channels allowing uniform read/write operations across OS abstractions.
* **TCP Server Lifecycle**: Executes `socket()`, configures `setsockopt` with `SO_REUSEADDR` to bypass `TIME_WAIT` constraints, binds the port (`bind()`), queues connections (`listen()`), and accepts clients (`accept()`).
* **Socket Separation**: Strict separation between the dedicated listening descriptor (`server_fd_`) and dynamic client connection descriptors (`client_fd`) to ensure thread-safe I/O isolation.

### 2. HTTP/1.1 Protocol Under the Hood
* **RFC 9112 Compliance**: Precise delimiter tracking with CRLF (`\r\n`) and double CRLF (`\r\n\r\n`) to separate headers from payload bodies.
* **Stream Deserialization**: Memory-safe parsing converting raw byte streams into structured C++ domain objects without buffer over-reads.
* **Content Management**: Strict MIME type classification with fallback to `application/octet-stream` and explicit `Content-Length` accounting.

### 3. Concurrency & Multithreading
* **Fixed Thread Pool Model**: Pre-allocates a deterministic pool of worker threads, avoiding the memory starvation and context-switching tax of Thread-per-Client designs.
* **RAII Synchronization**: Guards shared task queues via `std::mutex` and `std::unique_lock`, paired with `std::condition_variable` for efficient sleeping and signaling (`cv_.notify_one()`).
* **Spurious Wakeup & Race Protection**: Employs predicate lambdas to protect against spurious wakeups and maintains atomic state visibility during termination.
* **Graceful Shutdown**: Coordinates a synchronized exit sequence, releasing blocked workers and joining threads cleanly to prevent `std::terminate` or abrupt connection cuts.

### 4. Security & Static Serving
* **Path Traversal Mitigation**: Validates resolved paths against the base directory using `std::filesystem::relative` and explicit checks for directory escape patterns (`..`), guaranteeing isolation within `public/`.
* **Binary-Safe I/O**: Reads static assets in binary mode (`std::ios::binary` and `std::ios::ate`) to prevent platform-specific line-ending translations from corrupting images, scripts, or WASM binaries.

---

## Performance Benchmarks (k6 Load Testing)

Tested under local stress conditions with up to 100 concurrent Virtual Users (VUs) over a 2-minute sustained load:

| Metric | Result | Engineering Significance |
| :--- | :--- | :--- |
| **Total Requests Handled** | **62,737+** | Sustained traffic volume over 2 minutes without degradation |
| **Success Rate** | **100.00%** | **0 dropped connections**, 0 I/O failures, 0 server errors |
| **Throughput** | **~522.4 RPS** | Stable processing rate exceeding 520 requests/sec |
| **Average Latency** | **6.86 ms** | Fast response times including disk reads and network delivery |
| **Median Latency** | **5.29 ms** | Half of all requests answered in ~5 ms |
| **p(95) Latency** | **17.24 ms** | Consistent performance even at upper load bounds |

---

## How to Build & Run

```bash
# 1. Build project
mkdir -p build && cd build
cmake ..
cmake --build .

# 2. Run the Web Server
./nexus_server

# 3. Run Test Suites
./test_server_basic
./test_static_files