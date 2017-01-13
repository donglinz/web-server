#include "ServerBase.hpp"



template<typename socket_type>
WebServer::ServerBase<socket_type>::ServerBase(unsigned short port, size_t num_threads) :
	endpoint(boost::asio::ip::tcp::v4(), port),
	acceptor(m_io_service, endpoint),
	num_threads(num_threads) {}

template<typename socket_type>
WebServer::ServerBase<socket_type>::~ServerBase()
{
}

template<typename socket_type>
void WebServer::ServerBase<socket_type>::start()
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
		threads.emplace_back([this]() {
			m_io_service.run();
		});
	}

	m_io_service.run();
	
	for (auto &t : threads) {
		t.join();
	}
}

template<typename socket_type>
void WebServer::ServerBase<socket_type>::accept()
{
}

template<typename socket_type>
void WebServer::ServerBase<socket_type>::process_request_and_responce(std::shared_ptr<socket_type> socket) const
{
	// 为 async_read_untile() 创建新的读缓存
    // shared_ptr 用于传递临时对象给匿名函数
    // 会被推导为 std::shared_ptr<boost::asio::streambuf>
	auto read_buffer = std::make_shared<boost::asio::streambuf>();
	boost::asio::async_read_until(*socket, *read_buffer, "\r\n\r\n", 
		[this, socket, read_buffer](const boost::system::error_code& ec, size_t bytes_transferred) {
		if (!ec) {
			// 注意：read_buffer->size() 的大小并一定和 bytes_transferred 相等， Boost 的文档中指出：
            // 在 async_read_until 操作成功后,  streambuf 在界定符之外可能包含一些额外的的数据
            // 所以较好的做法是直接从流中提取并解析当前 read_buffer 左边的报头, 再拼接 async_read 后面的内容
			size_t total = read_buffer->size();
			// 转换到 istream
			std::istream stream(read_buffer.get());

			auto request = std::make_shared<Request>();

			*request = prase_request(stream);
			
			size_t num_additional_bytes = total - bytes_transferred;

			if (request->header.count("Content-Length")>0) {
				boost::asio::async_read(*socket, *read_buffer,
					boost::asio::transfer_exactly(stoull(request->header["Content-Length"]) - num_additional_bytes),
					[this, socket, read_buffer, request](const boost::system::error_code& ec, size_t bytes_transferred) {
					if (!ec) {
						// 将指针作为 istream 对象存储到 read_buffer 中
						request->content = std::make_shared<std::istream>(read_buffer.get());
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
WebServer::Request WebServer::ServerBase<socket_type>::prase_request(std::istream & stream) const
{
	Request request;
	// 使用正则表达式对请求报头进行解析，通过下面的正则表达式
    // 可以解析出请求方法(GET/POST)、请求路径以及 HTTP 版本
	std::regex regex("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");

	std::smatch sub_match;
	//从第一行中解析请求方法、路径和 HTTP 版本
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


template<typename socket_type>
void WebServer::ServerBase<socket_type>::respond(std::shared_ptr<socket_type> socket, std::shared_ptr<Request> request) const {
    // 对请求路径和方法进行匹配查找，并生成响应
	
    for(auto res_it: all_resources) {
        std::regex e(res_it->first);
        std::smatch sm_res;
        if(std::regex_match(request->path, sm_res, e)) {
            if(res_it->second.count(request->method)>0) {
                request->path_match = move(sm_res);

                // 会被推导为 std::shared_ptr<boost::asio::streambuf>
                auto write_buffer = std::make_shared<boost::asio::streambuf>();
                std::ostream response(write_buffer.get());
                res_it->second[request->method](response, *request);

                // 在 lambda 中捕获 write_buffer 使其不会再 async_write 完成前被销毁
                boost::asio::async_write(*socket, *write_buffer,
                [this, socket, request, write_buffer](const boost::system::error_code& ec, size_t bytes_transferred) {
                    // HTTP 持久连接(HTTP 1.1), 递归调用
                    if(!ec && stof(request->http_version)>1.05)
                        process_request_and_respond(socket);
                });
                return;
            }
        }
    }
	
}