cmake_minimum_required(VERSION 3.20)
project(nexus_server CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

include_directories(include)

find_package(Threads REQUIRED)

add_executable(nexus_server
        src/main.cpp
        src/HttpServer.cpp
        src/HttpRequestParser.cpp
        src/HttpResponse.cpp
        src/Router.cpp
        include/HttpServer.hpp
        include/HttpRequestParser.hpp
        include/HttpResponse.hpp
        include/Router.hpp
        include/WorkerThreadPool.hpp
        src/WorkerThreadPool.cpp
)
target_link_libraries(nexus_server PRIVATE Threads::Threads)

add_executable(test_server_basic
        tests/test_server_basic.cpp
        src/HttpServer.cpp
        src/HttpRequestParser.cpp
        src/HttpResponse.cpp
        src/Router.cpp
        include/HttpServer.hpp
        include/HttpRequestParser.hpp
        include/HttpResponse.hpp
        include/Router.hpp
)
target_link_libraries(test_server_basic PRIVATE Threads::Threads)
