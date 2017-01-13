#pragma once
#include "ServerBase.hpp"

namespace WebServer {

	// boost::asio::ip::tcp::socket对象，实际上就是一个 HTTP 的 Socket 连接
	typedef boost::asio::ip::tcp::socket HTTP;

	template<typename socket_type = HTTP>
	class ServerHttp : public ServerBase<socket_type>
	{
	public:
		ServerHttp(unsigned short port, size_t num_threads = 1);
		~ServerHttp();
	
	private:
		virtual void accept();
	};
}

