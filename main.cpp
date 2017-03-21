//============================================================================
// Name        : main.cpp
// Author      : Donglin Zhuang
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//#include <iostream>
//#include <cstring>
#include <cstdio>
#include <boost/asio.hpp>
#include "ServerHttp.hpp"
#include "ServerHttps.hpp"
//#include "handler.hpp"
typedef boost::asio::ip::tcp::socket HTTP;
typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> HTTPS;
#include "Initializer.h"
#include "Logger.h"
#include "handler.hpp"
#define ON "on"
#define OFF "off"
void onExit();
void onSig(int sig);
void regSig();
int main(int argc, char *argv[]) {
    regSig();
    Initializer::init();
    Logger::init(Initializer::config[Configurations::logPath]);
    IOSystem::init(Initializer::config[Configurations::notFoundFile],
                   Initializer::config[Configurations::enableCache],
                   Initializer::config[Configurations::cacheSize],
                   Initializer::config[Configurations::useRedisCache],
                   Initializer::config[Configurations::redisHost],
                   Initializer::config[Configurations::redisPort],
                   Initializer::config[Configurations::redisPass],
                   Initializer::config[Configurations::redisDataBaseId],
                   Initializer::config[Configurations::redisTTL]);

    unsigned short port = (unsigned short) std::stoul(Initializer::config[Configurations::port]);
    int num_threads = std::stoi(Initializer::config[Configurations::threadNum]);
    if(Initializer::config[Configurations::httpServer] == ON) {
        Handler<WebServer::ServerHttp<HTTP> > hd;
        WebServer::ServerHttp<HTTP> server(port, (size_t) num_threads);
        hd.start_server(server);
    } else {
        Handler<WebServer::ServerHttps<HTTPS> > hd;
        std::string publicKey = Initializer::config[Configurations::publicKey];
        std::string privateKey = Initializer::config[Configurations::privateKey];
        WebServer::ServerHttps<HTTPS> server(port, (size_t) num_threads, publicKey, privateKey);
        hd.start_server(server);
    }
	return 0;
}

void onExit() {
    RedisCacheManager::stop();
}

void onSig(int sig) {
    RedisCacheManager::stop();
}

void regSig() {
    atexit(onExit);
    signal(SIGABRT, onSig);
    signal(SIGFPE, onSig);
    signal(SIGILL, onSig);
    signal(SIGINT, onSig);
    signal(SIGSEGV, onSig);
    signal(SIGTERM, onSig);
}