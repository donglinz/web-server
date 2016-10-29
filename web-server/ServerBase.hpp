#pragma once
#include <unordered_map>
#include <thread>
#include <regex>
#include <map>
#include <functional>
#include <boost/asio.hpp>
#include <vector>

namespace WebServerBase{
	struct Request {
		std::string method, path, http_version;

		std::shared_ptr<std::istream> content;

		std::unordered_map<std::string, std::string> header;

		std::smatch path_match;

	};

	typedef std::map < std::string, std::unordered_map < std::string,
		std::function<void(std::ostream&, Request& )> > > resource_type;

	template<typename socket_type>
	class ServerBase
	{
	public:
		ServerBase(unsigned short port, size_t num_threads = 1);
		virtual ~ServerBase();

		void start();

	protected:
		virtual void accept();

		void process_request_and_responce(std::shared_ptr<socket_type> socket) const;

	public:
		resource_type resource;

		resource_type default_resource;

	protected:
		std::vector<resource_type::iterator> all_resources;

		boost::asio::io_service m_io_service;
		
		boost::asio::ip::tcp::endpoint endpoint;
		
		boost::asio::ip::tcp::acceptor acceptor;

		size_t num_threads;

		std::vector<std::thread> threads;

	};

	
}


