# Nexus Server 🚀

A lightweight, high-performance HTTP/1.1 web server built from scratch in **C++20** with zero external dependencies[cite: 1]. The architecture leverages POSIX Sockets for networking and a custom Thread Pool for concurrent execution[cite: 1]. It is designed as a robust backend capable of serving static assets (including WebAssembly and Frontend builds) and handling high traffic loads[cite: 1].

## Current Project Status
The server is currently at **75%-80% completion**[cite: 1]:
* **Completed**: Core network engine (`HttpServer`), HTTP parsing/serialization (`HttpRequestParser` & `HttpResponse`), declarative routing (`Router`), and the multi-threaded `WorkerThreadPool`[cite: 1].
* **In Progress (Final 10%)**: Static file serving (`StaticFileHandler`) and implementing Path Traversal security[cite: 1].
* **Next Steps (Final 10-15%)**: Load benchmarking (e.g., using `wrk`) and finalizing documentation[cite: 1].

## Theoretical Concepts & Architecture

### 1. Networking & POSIX Sockets
* **File Descriptors (fd)**: Treats network connections as standard I/O channels allowing standard read/write operations[cite: 2].
* **TCP Server Lifecycle**: Executes `socket()`, uses `setsockopt` with `SO_REUSEADDR` to bypass `TIME_WAIT`, binds the port (`bind()`), queues connections (`listen()`), and accepts clients (`accept()`)[cite: 2].
* **Data Flow**: Extracts raw bytes from the network into a buffer, processes the stream, and writes the response back[cite: 2].

### 2. HTTP/1.1 Protocol Under the Hood
* **Protocol Design**: Operates as a stateless, text-based Request-Response system[cite: 2].
* **Formatting rules**: Utilizes CRLF (`\r\n`) for line endings and a double CRLF (`\r\n\r\n`) to separate headers from the body payload[cite: 2].
* **Serialization/Deserialization**: Converts raw byte streams into logical C++ objects (`HttpRequestParser`) and serializes C++ objects back to compliant text (`HttpResponse`)[cite: 2].
* **Content Management**: Relies on `Content-Type` for MIME mapping and `Content-Length` for precise payload sizing[cite: 2].

### 3. Concurrency & Multithreading
* **Thread Pool Model**: Pre-allocates a fixed number of workers to prevent the overhead and resource exhaustion of a Thread-per-Client approach[cite: 2].
* **Synchronization Primitives**: Secures shared data via `std::mutex` and mitigates busy-waiting CPU cycles using `std::condition_variable` (`wait()`, `notify_one()`, `notify_all()`)[cite: 2].
* **RAII Locks**: Guarantees safe lock release during stack unwinding via `std::unique_lock`[cite: 2].
* **Graceful Shutdown**: Safely signals a termination flag, wakes sleeping threads, and halts execution via `join()` to prevent resource leaks[cite: 2].

### 4. Software Engineering & Security
* **Single Responsibility Principle (SRP)**: Strictly separates network lifecycle (`HttpServer`), data parsing (`HttpRequestParser`), data modeling (`HttpResponse`), and logic dispatching (`Router`)[cite: 2].
* **Path Traversal Mitigation**: Secures file serving by canonicalizing requested paths and verifying they strictly reside within the designated `public/` directory[cite: 2].

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
