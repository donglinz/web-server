#pragma once
#include <unordered_map>
#include <thread>
#include <regex>
#include <boost/asio.hpp>

namespace WebServer{
	
	struct Request {
		// 请求方法, POST, GET; 请求路径; HTTP 版本
		std::string method, path, http_version;
		// 对 content 使用智能指针进行引用计数
		std::shared_ptr<std::istream> content;
		// 哈希容器, key-value 字典
		std::unordered_map<std::string, std::string> header;
		// 用正则表达式处理路径匹配
		std::smatch path_match;

	};

	typedef std::map < std::string, 
		std::unordered_map < std::string,
			std::function<void(std::ostream&, Request& )> 
		> 
	> resource_type;


	// http or https
	template<typename socket_type>
	class ServerBase
	{
	public:
		explicit ServerBase(unsigned short port, size_t num_threads = 1);

		virtual ~ServerBase();
		// 启动服务器
		void start();
	
		resource_type resource;
		
		resource_type default_resource;

	protected:
		// 所有的资源及默认资源都会在 vector 尾部添加, 并在 start() 中创建
		std::vector<resource_type::iterator> all_resources;
		// asio 库中的 io_service 是调度器，所有的异步 IO 事件都要通过它来分发处理
        // 换句话说, 需要 IO 的对象的构造函数，都需要传入一个 io_service 对象
		boost::asio::io_service m_io_service;
		// IP 地址、端口号、协议版本构成一个 endpoint，并通过这个 endpoint 在服务端生成
        // tcp::acceptor 对象，并在指定端口上等待连接
		boost::asio::ip::tcp::endpoint endpoint;
		// 所以，一个 acceptor 对象的构造都需要 io_service 和 endpoint 两个参数
		boost::asio::ip::tcp::acceptor acceptor;

		size_t num_threads;

		std::vector<std::thread> threads;
		
		virtual void accept();
		// implemention request and reponse
		void process_request_and_responce(std::shared_ptr<socket_type> socket) const;
	
		Request prase_request(std::istream& stream) const;

		void respond(std::shared_ptr<socket_type> socket, std::shared_ptr<Request> request) const;
	};	

}


