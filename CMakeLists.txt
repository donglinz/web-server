cmake_minimum_required(VERSION 3.5.1)
project(web_server)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        handler.hpp
        main.cpp
        ServerBase.hpp
        ServerHttp.hpp
        ServerHttps.hpp)
link_libraries(-lboost_system -lssl -lcrypto -lpthread)
add_executable(web_server ${SOURCE_FILES})