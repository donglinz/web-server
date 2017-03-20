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
    disConnectCallback = [&disConnectCallback](cpp_redis::redis_client & client)->void {
        Logger::LogError("Redis Client DisConnected from host " +
                                 host + ":" + port + " Reconnecting...");
        try {
            client.connect(host, std::stoul(port), disConnectCallback);
        } catch( ... ) {
            Logger::LogError("Redis client error, cannot connect to host " +
            host + ":" + port);
        }
    };
    try {
        client.connect(host, std::stoul(port), disConnectCallback);
    } catch( ... ) {
        Logger::LogError("Redis client error, cannot connect to host " +
                         host + ":" + port);
    }
}

void RedisCacheManager::setTimer(boost::posix_time::microsec timeInterval) {
    std::shared_ptr<boost::asio::deadline_timer> timerPtr =
            std::make_shared<boost::asio::deadline_timer>(io_service, timeInterval);
    timerPtr->async_wait([timerPtr, &io_service, &client](const boost::system::error_code & ec)->void {
        client.commit();
        setTimer(timeInterval);
    });
}

void RedisCacheManager::asyncResponse(std::ostream response, std::string fileName, std::function<void()> callback) {

}

