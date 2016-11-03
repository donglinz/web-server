#pragma once
#include <unordered_map>
#include <thread>
#include <regex>
#include <map>
#include <functional>
#include <boost/asio.hpp>
#include <vector>
#include <ostream>

namespace WebServerBase{
	
	struct Request {
		// 请求方法(post, get), 请求路径, http版本
		std::string method, path, http_version;
		
		std::shared_ptr<std::istream> content;

		std::unordered_map<std::string, std::string> header;
		// 正则表达式处理路径匹配
		std::smatch path_match;

	};

	typedef std::map < std::string, std::unordered_map < std::string,
		std::function<void(std::ostream&, Request& )> > > resource_type;


	// http or https
	template<typename socket_type>
	class ServerBase
	{
	public:
		explicit ServerBase(unsigned short port, size_t num_threads = 1);

		virtual ~ServerBase();
		// 启动服务器
		void start();
		void hehehe() = delete;

	protected:

		virtual void accept();
		// implemention request and reponse
		void process_request_and_responce(std::shared_ptr<socket_type> socket) const;
	
		Request prase_Request(std::istream& stream) const;
	public:
		// 用于服务器访问资源处理方式
		resource_type resource;
		// 用于保存默认资源的处理方式
		resource_type default_resource;

	protected:
		// 实现对内部所有资源的处理，资源都在start中创建
		std::vector<resource_type::iterator> all_resources;
		/*
		asio 库中的 io_service 是调度器，所有的异步 IO 事件都要通过它来分发处理
		需要 IO 的对象的构造函数，都需要传入一个 io_service 对象
		*/
		boost::asio::io_service m_io_service;
		// IP 地址、端口号、协议版本构成一个 endpoint，并通过这个 endpoint 在服务端生成
		boost::asio::ip::tcp::endpoint endpoint;
		// tcp::acceptor 对象，并在指定端口上等待连接， 构造需要io_service 和endpoing两个参数
		boost::asio::ip::tcp::acceptor acceptor;

		size_t num_threads;

		std::vector<std::thread> threads;
		
	};

	
}


