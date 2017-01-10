#pragma once
#include "ServerBase.hpp"

namespace WebServerBase {
	template<typename socket_type>
	class Server :
		public ServerBase<socket_type>
	{
	public:
		Server(unsigned short port, size_t num_threads);
		~Server();
	};
}

