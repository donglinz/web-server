#include <iostream>
#include <fstream>
#include "ServerHttp.hpp"
template<typename SERVER_TYPE>
class Handler{
public:
    void start_server(SERVER_TYPE & server);
};