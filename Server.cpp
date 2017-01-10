#include "Server.hpp"




template<typename socket_type>
WebServerBase::Server<socket_type>::Server(unsigned short port, size_t num_threads) : Server(port, num_threads)
{
}

template<typename socket_type>
WebServerBase::Server<socket_type>::~Server()
{
}