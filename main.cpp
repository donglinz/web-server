//============================================================================
// Name        : main.cpp
// Author      : Donglin Zhuang
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "ServerHttp.hpp"
typedef boost::asio::ip::tcp::socket HTTP;

int main() {
	WebServer::ServerHttp<HTTP> server(12345, 4);
	
	return 0;
}
