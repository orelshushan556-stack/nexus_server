# Nexus Server 🚀

A lightweight, high-performance HTTP/1.1 web server built from scratch in **C++20** with zero external dependencies. The architecture leverages POSIX Sockets for networking and a custom Thread Pool for concurrent execution. It is designed as a robust backend capable of serving static assets (including WebAssembly and Frontend builds) and handling high traffic loads.

## Current Project Status
The server is currently at **75%-80% completion**:
* **Completed**: Core network engine (`HttpServer`), HTTP parsing/serialization (`HttpRequestParser` & `HttpResponse`), declarative routing (`Router`), and the multi-threaded `WorkerThreadPool` with graceful shutdown.
* **In Progress**: Static file serving (`StaticFileHandler`) - Path Traversal security is implemented; final serving logic is next.
* **Next Steps**: Load benchmarking (e.g., using `wrk`) and finalizing documentation.

## Theoretical Concepts & Architecture

### 1. Networking & POSIX Sockets
* **File Descriptors (fd)**: Treats network connections as standard I/O channels allowing standard read/write operations.
* **TCP Server Lifecycle**: Executes `socket()`, uses `setsockopt` with `SO_REUSEADDR` to bypass `TIME_WAIT`, binds the port (`bind()`), queues connections (`listen()`), and accepts clients (`accept()`).
* **Data Flow**: Extracts raw bytes from the network into a buffer, processes the stream, and writes the response back.

### 2. HTTP/1.1 Protocol Under the Hood
* **Protocol Design**: Operates as a stateless, text-based Request-Response system.
* **Formatting rules**: Utilizes CRLF (`\r\n`) for line endings and a double CRLF (`\r\n\r\n`) to separate headers from the body payload.
* **Serialization/Deserialization**: Converts raw byte streams into logical C++ objects (`HttpRequestParser`) and serializes C++ objects back to compliant text (`HttpResponse`).
* **Content Management**: Relies on `Content-Type` for MIME mapping and `Content-Length` for precise payload sizing.

### 3. Concurrency & Multithreading
* **Thread Pool Model**: Pre-allocates a fixed number of workers to prevent the overhead and resource exhaustion of a Thread-per-Client approach.
* **Synchronization Primitives**: Secures shared data via `std::mutex` and mitigates busy-waiting CPU cycles using `std::condition_variable` (`wait()`, `notify_one()`, `notify_all()`).
* **RAII Locks**: Guarantees safe lock release during stack unwinding via `std::unique_lock`.
* **Graceful Shutdown**: Safely signals a termination flag, wakes sleeping threads, and halts execution via `join()` to prevent resource leaks.

### 4. Software Engineering & Security
* **Single Responsibility Principle (SRP)**: Strictly separates network lifecycle (`HttpServer`), data parsing (`HttpRequestParser`), data modeling (`HttpResponse`), and logic dispatching (`Router`).
* **Path Traversal Mitigation**: Secures file serving by canonicalizing requested paths and verifying they strictly reside within the designated `public/` directory.

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
