cmake_minimum_required(VERSION 3.5.1)
project(web_server)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-DBOOST_LOG_DYN_LINK)
set(SOURCE_FILES
        handler.hpp
        main.cpp
        ServerBase.hpp
        ServerHttp.hpp
        ServerHttps.hpp
        xml/tinyxml2.h
        xml/tinyxml2.cpp init/Initializer.cpp init/Initializer.h cache/CacheManager.cpp cache/CacheManager.h log/Logger.cpp log/Logger.h)
include_directories(xml init log cache)
link_libraries(-lboost_system -lssl -lcrypto -lpthread
        -lboost_log -lboost_thread -lboost_log_setup)
add_executable(web_server ${SOURCE_FILES})