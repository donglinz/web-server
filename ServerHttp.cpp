#include "ServerHttp.hpp"




template<typename socket_type>
WebServer::ServerHttp<socket_type>::ServerHttp(unsigned short port, size_t num_threads) : ServerBase<socket_type>(port, num_threads)
{
    
}

template<typename socket_type>
WebServer::ServerHttp<socket_type>::~ServerHttp()
{
}