cmake_minimum_required(VERSION 3.5.1)
project(web_server)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        handler.hpp
        main.cpp
        ServerBase.hpp
        ServerHttp.hpp
        ServerHttps.hpp
        xml/tinyxml2.h
        xml/tinyxml2.cpp init/Initializer.cpp init/Initializer.h)
include_directories(xml init log)
link_libraries(-lboost_system -lssl -lcrypto -lpthread)
add_executable(web_server ${SOURCE_FILES})