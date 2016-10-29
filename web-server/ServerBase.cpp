#include "ServerBase.hpp"



template<typename socket_type>
WebServerBase::ServerBase<socket_type>::ServerBase(unsigned short port, size_t num_threads) :
	endpoint(boost::asio::ip::tcp::v4(), port),
	acceptor(m_io_service, endpoing),
	num_threads(num_threads) {}

template<typename socket_type>
WebServerBase::ServerBase<socket_type>::~ServerBase()
{
}

template<typename socket_type>
void WebServerBase::ServerBase<socket_type>::start()
{
}

template<typename socket_type>
void WebServerBase::ServerBase<socket_type>::accept()
{
}

template<typename socket_type>
void WebServerBase::ServerBase<socket_type>::process_request_and_responce(std::shared_ptr<socket_type> socket) const
{
}
