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
        XMLReader/tinyxml2.h
        XMLReader/tinyxml2.cpp Init/Initializer.cpp Init/Initializer.h
        CacheSystem/CacheManager.cpp CacheSystem/CacheManager.h Logger/Logger.cpp Logger/Logger.h)
include_directories(XMLReader Init Logger CacheSystem)
link_libraries(-lboost_system -lssl -lcrypto -lpthread
        -lboost_log -lboost_thread -lboost_log_setup)
add_executable(web_server ${SOURCE_FILES})