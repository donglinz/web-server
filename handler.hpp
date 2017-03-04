#include <iostream>
#include <fstream>
#include "ServerHttp.hpp"
template<typename SERVER_TYPE>
class Handler{
public:
    Handler();
    ~Handler();
    void start_server(SERVER_TYPE & server);
};

template<typename SERVER_TYPE>
Handler<SERVER_TYPE>::Handler() {

}
template<typename SERVER_TYPE>
Handler<SERVER_TYPE>::~Handler() {

}
template<typename SERVER_TYPE>
void Handler<SERVER_TYPE>::start_server(SERVER_TYPE & server) {
    // 向服务器增加请求资源的处理方法
    // 运行 HTTP 服务器
    server.start();
}
