//============================================================================
// Name        : main.cpp
// Author      : Donglin Zhuang
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "ServerHttp.hpp"
#include "handler.hpp"
typedef boost::asio::ip::tcp::socket HTTP;

int main() {
    Handler<WebServer::ServerHttp<HTTP> > hd;
    WebServer::ServerHttp<HTTP> server(12345, 4);
    hd.start_server(server);
	return 0;
}
