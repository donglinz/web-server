#pragma once
#include <unordered_map>
#include <thread>
#include <regex>
#include <boost/asio.hpp>
#include <ostream>
#include <fstream>
#include <memory>
#include <algorithm>

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

        void stop();

        void add_resource(const std::string & path, const std::string & method, std::function<void(std::ostream&, Request&)>);

        void add_default_resource(const std::string & path, const std::string & method, std::function<void(std::ostream&, Request&)>);

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

        resource_type resource;

        resource_type default_resource;

        size_t num_threads;

        std::vector<std::thread> threads;

        virtual void accept();
        // implemention request and reponse
        void process_request_and_responce(std::shared_ptr<socket_type> socket) const;

        std::shared_ptr<Request> prase_request(std::istream& stream) const;

        void respond(std::shared_ptr<socket_type> socket, std::shared_ptr<Request> request) const;

    private:
        void init_resource();

        void not_found(std::ostream & responce);

        //std::string root_directory;

        std::regex re_path_contain_file = std::regex("^([a-zA-Z0-9/._-]+)/([a-zA-Z0-9._-]+)\\.([a-z]+)$");
        //std::regex re_is_index = std::regex("^web/([a-zA-Z0-9._-]+/)?([a-zA-Z0-9./_-]*)index.html$");
    };
    template<typename socket_type>
    WebServer::ServerBase<socket_type>::ServerBase(unsigned short port, size_t num_threads) :
            endpoint(boost::asio::ip::tcp::v4(), port),
            acceptor(m_io_service, endpoint),
            num_threads(num_threads) {
        init_resource();
    }

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
    void WebServer::ServerBase<socket_type>::stop() {

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

                                              std::shared_ptr<Request> request =  prase_request(stream);

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
    std::shared_ptr<Request> WebServer::ServerBase<socket_type>::prase_request(std::istream & stream) const
    {
        std::shared_ptr<Request> request = std::make_shared<Request>(WebServer::Request());
        // 使用正则表达式对请求报头进行解析，通过下面的正则表达式
        // 可以解析出请求方法(GET/POST)、请求路径以及 HTTP 版本
        std::regex regex("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");

        std::smatch sub_match;
        //从第一行中解析请求方法、路径和 HTTP 版本
        std::string line;

        std::getline(stream, line);
        line.pop_back();
        if (std::regex_match(line, sub_match, regex)) {
            request->method = sub_match[1];
            request->path = sub_match[2];
            request->http_version = sub_match[3];
            if((request->path).find('?') != std::string::npos) {
                request->path = (request->path).substr(0, (request->path).find('?'));
            }
            bool matched = false;
            regex = "^([^:]*): ?(.*)$";
            do {
                getline(stream, line);
                line.pop_back();
                matched = std::regex_match(line, sub_match, regex);
                if (matched) {
                    request->header[sub_match[1]] = sub_match[2];
                }
            } while (matched);
        }
        return request;
    }


    template<typename socket_type>
    void WebServer::ServerBase<socket_type>::respond(std::shared_ptr<socket_type> socket, std::shared_ptr<Request> request) const {
        // 对请求路径和方法进行匹配查找，并生成响应

        for(auto res_it: all_resources) {
            std::regex e(res_it->first);
            std::smatch sm_res;
            if(regex_match(request->path, sm_res, e)) {
                if(res_it->second.count(request->method)>0) {
                    request->path_match = move(sm_res);


                    std::shared_ptr<boost::asio::streambuf> write_buffer = std::make_shared<boost::asio::streambuf>();
                    std::ostream response(write_buffer.get());
                    res_it->second[request->method](response, *request);

                    // 在 lambda 中捕获 write_buffer 使其不会再 async_write 完成前被销毁 (666
                    boost::asio::async_write(*socket, *write_buffer,
                                             [this, socket, request, write_buffer](const boost::system::error_code& ec, size_t bytes_transferred) {
                                                 // HTTP 持久连接(HTTP 1.1), 递归调用
                                                 if(!ec && stod(request->http_version)>1.05)
                                                     process_request_and_responce(socket);
                                             });
                    return;
                }
            }
        }
    }

    template<typename socket_type>
    void ServerBase<socket_type>::add_resource(const std::string & path, const std::string & method, std::function<void(std::ostream&, Request&)> func) {
        resource[path][method] = func;
    }

    template<typename socket_type>
    void ServerBase<socket_type>::add_default_resource(const std::string & path, const std::string & method, std::function<void(std::ostream&, Request&)> func) {
        default_resource[path][method] = func;
    }

    template<typename socket_type>
    void ServerBase<socket_type>::init_resource() {
        // 处理访问 /string 的 POST 请求，返回 POST 的字符串
        add_resource("^/string/?$", "POST",
                     [](std::ostream& response, WebServer::Request& request) {
                         // 从 istream 中获取字符串 (*request.content)
                         std::stringstream ss;
                         *request.content >> ss.rdbuf();     // 将请求内容读取到 stringstream
                         std::string content=ss.str();

                         // 直接返回请求结果
                         response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
                     });

        // 处理访问 /info 的 GET 请求, 返回请求的信息
        add_resource("^/info/?$", "GET",
                     [](std::ostream& response, WebServer::Request& request) {
                         std::stringstream content_stream;
                         content_stream << "<h1>Request:</h1>";
                         content_stream << request.method << " " << request.path << " HTTP/" << request.http_version << "<br>";
                         for(auto& header: request.header) {
                             content_stream << header.first << ": " << header.second << "<br>";
                         }

                         // 获得 content_stream 的长度(使用 content.tellp() 获得)
                         content_stream.seekp(0, std::ios::end);

                         response <<  "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n" << content_stream.rdbuf();
                     });

        // 处理访问 /match/[字母+数字组成的字符串] 的 GET 请求, 例如执行请求 GET /match/abc123, 将返回 abc123
        add_resource("^/match/([0-9a-zA-Z]+)/?$", "GET",
                     [](std::ostream& response, WebServer::Request& request) {
                         std::string number=request.path_match[1];
                         response << "HTTP/1.1 200 OK\r\nContent-Length: " << number.length() << "\r\n\r\n" << number;
                     } );

        // 处理默认 GET 请求, 如果没有其他匹配成功，则这个匿名函数会被调用
        // 将应答 web/ 目录及其子目录中的文件
        // 默认文件: index.html
        add_default_resource("^/?(.*)$", "GET",
                             [this](std::ostream& response, WebServer::Request& request) {
                                 // root directory of web resource
                                 std::string filename = "web";

                                 std::string path = request.path_match[0];

                                 std::smatch sub_match;
                                 filename += path;

                                 if(strstr(filename.c_str(), "/../") != nullptr) {
                                     not_found(response);
                                     return;
                                 }

                                 if(!std::regex_match(filename, re_path_contain_file)) {
                                     if(filename.back() != '/') {
                                         filename += "/";
                                     }
                                     filename += "index.html";
                                 }

                                 filename = filename.substr(0, filename.find('?'));


                                 std::ifstream ifs;
                                 ifs.open(filename, std::ifstream::in | std::ifstream::binary);

                                 if(ifs) {
                                     ifs.seekg(0, std::ios::end);
                                     size_t length = (size_t) ifs.tellg();

                                     ifs.seekg(0, std::ios::beg);

                                     // 文件内容拷贝到 response-stream 中，不应该用于大型文件
                                     response << "HTTP/1.1 200 OK\r\nContent-Length: " << length << "\r\n\r\n" << ifs.rdbuf();

                                     ifs.close();
                                 } else {
                                     // 文件不存在时，返回无法打开文件
                                      std::string content="Could not open file " + filename;
                                      response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
                                     //not_found(response);
                                 }
                             });
    }

    template<typename socket_type>
    void ServerBase<socket_type>::not_found(std::ostream & responce) {
        responce << "HTTP/1.1 404 Not Found\r\n";
    }
}


