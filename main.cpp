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
#include "CacheManager.h"
#include "handler.hpp"
#define ON "on"
#define OFF "off"
int main(int argc, char *argv[]) {
    Initializer::init();
    Logger::init(Initializer::config[Configurations::logPath]);
    CacheManager::init(Initializer::config[Configurations::enableCache],
         Initializer::config[Configurations::cacheSize]);
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

//    if(strcmp(argv[1], "ServerHttps") == 0) {
//        std::cout << "https Server" << "\n" <<
//        "port : " <<  std::stoi(argv[2]) << "\n" <<
//        "thread num : " << std::stoi(argv[3]) << "\n" <<
//        "public key file : " << argv[4] << "\n" <<
//        "private key file : " << argv[5] << "\n" << std::endl;
//        std::cout << "continue?(Y\\N)" << std::endl;
//        char op;
//        std::cin >> op;
//        if(op == 'n' || op == 'N') {
//            std::cout << "Abort." << std::endl;
//            exit(0);
//        }
//        std::cout << "server run at port " << std::stoi(argv[2]) << std::endl;
//        Handler<WebServer::ServerHttps<HTTPS> > hd;
//        WebServer::ServerHttps<HTTPS> server(std::stoi(argv[2]), std::stoi(argv[3]), argv[4], argv[5]);
//        hd.start_server(server);
//    }
//    if(strcmp(argv[1], "ServerHttp") == 0) {
//        std::cout << "http Server" << "\n" <<
//        "port : " <<  std::stoi(argv[2]) << "\n" <<
//        "thread num : " << std::stoi(argv[3]) << "\n" << std::endl;
//        std::cout << "continue?(Y\\N)" << std::endl;
//        char op;
//        std::cin >> op;
//        if(op == 'n' || op == 'N') {
//            std::cout << "Abort." << std::endl;
//            exit(0);
//        }
//        std::cout << "server run at port " << std::stoi(argv[2]) << std::endl;
//        Handler<WebServer::ServerHttp<HTTP> > hd;
//        WebServer::ServerHttp<HTTP> server(std::stoi(argv[2]), std::stoi(argv[3]));
//        hd.start_server(server);
//    }
	return 0;
}
