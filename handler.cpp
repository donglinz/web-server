#include "handler.hpp"

template<typename SERVER_TYPE>
void Handler<SERVER_TYPE>::start_server(SERVER_TYPE & server) {
    // 向服务器增加请求资源的处理方法

    // 处理访问 /string 的 POST 请求，返回 POST 的字符串
    server.resource["^/string/?$"]["POST"] = [](std::ostream& response, WebServer::Request& request) {
        // 从 istream 中获取字符串 (*request.content)
        std::stringstream ss;
        *request.content >> ss.rdbuf();     // 将请求内容读取到 stringstream
        std::string content=ss.str();

        // 直接返回请求结果
        response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
    };

    // 处理访问 /info 的 GET 请求, 返回请求的信息
    server.resource["^/info/?$"]["GET"] = [](std::ostream& response, WebServer::Request& request) {
        std::stringstream content_stream;
        content_stream << "<h1>Request:</h1>";
        content_stream << request.method << " " << request.path << " HTTP/" << request.http_version << "<br>";
        for(auto& header: request.header) {
            content_stream << header.first << ": " << header.second << "<br>";
        }

        // 获得 content_stream 的长度(使用 content.tellp() 获得)
        content_stream.seekp(0, std::ios::end);

        response <<  "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n" << content_stream.rdbuf();
    };

    // 处理访问 /match/[字母+数字组成的字符串] 的 GET 请求, 例如执行请求 GET /match/abc123, 将返回 abc123
    server.resource["^/match/([0-9a-zA-Z]+)/?$"]["GET"] = [](std::ostream& response, WebServer::Request& request) {
        std::string number=request.path_match[1];
        response << "HTTP/1.1 200 OK\r\nContent-Length: " << number.length() << "\r\n\r\n" << number;
    };

    // 处理默认 GET 请求, 如果没有其他匹配成功，则这个匿名函数会被调用
    // 将应答 web/ 目录及其子目录中的文件
    // 默认文件: index.html
    server.default_resource["^/?(.*)$"]["GET"] = [](std::ostream& response, WebServer::Request& request) {
        std::string filename = "web/";

        std::string path = request.path_match[1];

        // 防止使用 `..` 来访问 web/ 目录外的内容
        size_t last_pos = path.rfind(".");
        size_t current_pos = 0;
        size_t pos;
        while((pos=path.find('.', current_pos)) != std::string::npos && pos != last_pos) {
            current_pos = pos;
            path.erase(pos, 1);
            last_pos--;
        }

        filename += path;
        std::ifstream ifs;
        // 简单的平台无关的文件或目录检查
        if(filename.find('.') == std::string::npos) {
            if(filename[filename.length()-1]!='/')
                filename+='/';
            filename += "index.html";
        }
        ifs.open(filename, std::ifstream::in);

        if(ifs) {
            ifs.seekg(0, std::ios::end);
            size_t length=ifs.tellg();

            ifs.seekg(0, std::ios::beg);

            // 文件内容拷贝到 response-stream 中，不应该用于大型文件
            response << "HTTP/1.1 200 OK\r\nContent-Length: " << length << "\r\n\r\n" << ifs.rdbuf();

            ifs.close();
        } else {
            // 文件不存在时，返回无法打开文件
            std::string content="Could not open file "+filename;
            response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
        }
    };

    // 运行 HTTP 服务器
    server.start();
}