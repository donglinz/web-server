//============================================================================
// Name        : main.cpp
// Author      : Donglin Zhuang
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "ServerHttp.hpp"
#include "ServerHttps.hpp"
#include "handler.hpp"
typedef boost::asio::ip::tcp::socket HTTP;
typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> HTTPS;
int main() {
    std::ios::sync_with_stdio(false);
    Handler<WebServer::ServerHttps<HTTPS> > hd;
    WebServer::ServerHttps<HTTPS> server((unsigned short) 12345, 4, "server.crt", "server.key");
    hd.start_server(server);

	return 0;
}
