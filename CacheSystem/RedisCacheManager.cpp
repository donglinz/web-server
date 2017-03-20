//
// Created by ubuntu on 17-3-19.
//

#include "RedisCacheManager.h"


std::string RedisCacheManager::getReadBuffer(std::string &fileName, size_t &ret_length) {
    return std::__cxx11::string();
}

bool RedisCacheManager::getCacheIsOpen() {
    return false;
}

void RedisCacheManager::init(std::string redisHost,
                             std::string redisPort,
                             std::string redisPass,
                             std::string redisDataBaseId,
                             std::string redisTTL) {
    host = redisHost;
    port = redisPort;
    pass = redisPass;
    dataBaseId = redisDataBaseId;
    TTL = redisTTL;
    connect();
}

void RedisCacheManager::connect() {
    asyncConnectionHandler = redisAsyncConnect(host, std::stoi(port));
    auto base = event_base_new();
    redisLibeventAttach(c, base);
    std::thread(event_base_dispatch, base).detach();
    if(c->err) {
        printf("Err: %s\n", c->errstr);
    }

    redisAsyncSetConnectCallback(c, [](const redisAsyncContext* conn, int status)->void {
        if(status == REDIS_OK) {
            std::cout << "conn ok" << std::endl;
        } else if(status == REDIS_ERR) std::cout << "conn error" << std::endl;
    });
    redisAsyncSetDisconnectCallback(c, [](const redisAsyncContext* conn, int status)->void {
        if(status == REDIS_OK) {
            std::cout << "disconn ok" << std::endl;
        } else if(status == REDIS_ERR) std::cout << "disconn error" << std::endl;
    });
}

//bool RedisCacheManager::isConnected(int id) {
//    if(connectHandler == nullptr || connectHandler->err) return false;
//    auto reply = static_cast<redisReply *>(redisCommand(connectHandler, "ping");
//    return !(reply->type != REDIS_REPLY_STATUS || strncmp(reply->str, "PONG", (size_t) std::min(reply->len, 4)));
//}

std::string RedisCacheManager::getErrorMsg(std::string msg) {
    return "Redis Error! " + msg;
}

redisReply *RedisCacheManager::redisCommandWapper(std::string command) {

    return nullptr;
}
