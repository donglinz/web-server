#include "ServerHttp.hpp"




template<typename socket_type>
WebServer::ServerHttp<socket_type>::ServerHttp(unsigned short port, size_t num_threads) : ServerBase<socket_type>::ServerBase(port, num_threads)
{

}

template<typename socket_type>
WebServer::ServerHttp<socket_type>::~ServerHttp()
{
    ServerBase<socket_type>::~ServerBase();
}

template<typename socket_type>
void WebServer::ServerHttp<socket_type>::accept() {
    // 为当前连接创建一个新的 socket
    // Shared_ptr 用于传递临时对象给匿名函数
    // socket 会被推导为 std::shared_ptr<HTTP> 类型
    auto socket = std::make_shared<HTTP>(ServerBase<socket_type>::m_io_service);

    ServerBase<socket_type>::acceptor.async_accept(*socket, [this, socket](const boost::system::error_code& ec) {
        // 立即启动并接受一个连接
        accept();
        // 如果出现错误
        if(!ec) process_request_and_respond(socket);
    });
}