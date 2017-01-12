#include "ServerBase.hpp"



template<typename socket_type>
WebServerBase::ServerBase<socket_type>::ServerBase(unsigned short port, size_t num_threads) :
	endpoint(boost::asio::ip::tcp::v4(), port),
	acceptor(m_io_service, endpoint),
	num_threads(num_threads) {}

template<typename socket_type>
WebServerBase::ServerBase<socket_type>::~ServerBase()
{
}

template<typename socket_type>
void WebServerBase::ServerBase<socket_type>::start()
{
	// 先匹配特殊资源处理方式，没有匹配的话用默认资源处理方式
	for (auto it = resource.begin(); it != resource.end(); ++it) {
		all_resources.push_back(it);
	}
	for (auto it = default_resource.begin(); it != default_resource.end(); ++it) {
		all_resources.push_back(it);
	}

	// 调用子类连接方式
	accept();

	for (size_t i = 1; i < num_threads; ++i) {
		threads.push_back([this]() {
			m_io_service.run();
		});
	}

	m_io_service.run();
	
	for (auto &t : threads) {
		t.join();
	}
}

template<typename socket_type>
void WebServerBase::ServerBase<socket_type>::accept()
{
}

template<typename socket_type>
void WebServerBase::ServerBase<socket_type>::process_request_and_responce(std::shared_ptr<socket_type> socket) const
{
	auto read_buffer = std::make_shared<boost::asio::streambuf>();
	boost::asio::async_read_until(*socket, *read_buffer, "\r\n\r\n", 
		[this, socket, read_buffer](const boost::system::error_code& ec, size_t bytes_transferred) {
		if (!ec) {
			size_t total = read_buffer->size();

			std::istream stream(read_buffer.get());

			auto request = std::make_shared<Request>();

			*request = parse_request(stream);
			
			size_t num_additional_bytes = total - bytes_transferred;

			if (request->header.count("Content-Length")>0) {
				boost::asio::async_read(*socket, *read_buffer,
					boost::asio::transfer_exactly(stoull(request->header["Content-Length"]) - num_additional_bytes),
					[this, socket, read_buffer, request](const boost::system::error_code& ec, size_t bytes_transferred) {
					if (!ec) {
						// 将指针作为 istream 对象存储到 read_buffer 中
						request->content = std::shared_ptr<std::istream>(new std::istream(read_buffer.get()));
						respond(socket, request);
					}
				});
			}
			else {
				respond(socket, request);
			}

		}
	});
}


template<typename socket_type>
WebServerBase::Request WebServerBase::ServerBase<socket_type>::prase_Request(std::istream & stream) const
{
	Request request;
	std::regex regex("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");

	std::smatch sub_match;

	std::string line;

	getline(stream, line);
	line.pop_back();
	if (std::regex_match(line, sub_match, regex)) {
		request.method = sub_match[1];
		request.path = sub_match[2];
		request.http_version = sub_match[3];
		
		bool matched;
		regex = "^([^:]*): ?(.*)$";
		do {
			getline(stream, line);
			line.pop_back();
			matched = std::regex_match(line, sub_match, regex);
			if (matched) {
				request.header[sub_match[1]] = sub_match[2];
			}
		} while (matched == true);

	}

	return request;
}
