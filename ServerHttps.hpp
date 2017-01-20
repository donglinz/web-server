#pragma once
#include "ServerBase.hpp"
#include <boost/asio/ssl.hpp>
namespace WebServer {
    typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> HTTPS;
    template<typename socket_type = HTTPS>
    class ServerHttps : public ServerBase<socket_type> {
    public:
        ServerHttps(unsigned short port,
                    size_t num_threads,
                    const std::string cert_file,
                    const std::string &private_key_file);
        ~ServerHttps();

    private:
        // HTTPS 会在 socket 这一步中对 IO 流进行加密
        // 因此实现的 accept() 方法需要对 socket 用 ssl context 初始化
        virtual void accept();

        boost::asio::ssl::context context;
    };
    template<typename socket_type>
    ServerHttps<socket_type>::ServerHttps(unsigned short port,
                                          size_t num_threads,
                                          const std::string cert_file,
                                          const std::string &private_key_file) :
            ServerBase<socket_type>::ServerBase(port, num_threads),
            context(boost::asio::ssl::context::sslv23)
    {
        context.use_certificate_chain_file(cert_file);
        context.use_private_key_file(private_key_file, boost::asio::ssl::context::pem);
    }
    template<typename socket_type>
    ServerHttps<socket_type>::~ServerHttps() {

    }
    template<typename socket_type>
    void ServerHttps<socket_type>::accept() {
        std::shared_ptr<HTTPS> socket = std::make_shared<HTTPS>(ServerBase<socket_type>::m_io_service, context);

        ServerBase<socket_type>::acceptor.async_accept((*socket).lowest_layer(),
        [this, socket](const boost::system::error_code & ec) {
            // 立即启动并接受一个新连接
            accept();
            if(!ec) {
                (*socket).async_handshake(boost::asio::ssl::stream_base::server,
                [this, socket](const boost::system::error_code & ec) {
                    if(!ec) ServerBase<socket_type>::process_request_and_responce(socket);
                });
            }
        });
    }
}