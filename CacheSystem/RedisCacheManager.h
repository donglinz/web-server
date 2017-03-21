//
// Created by ubuntu on 17-3-19.
//

#ifndef WEB_SERVER_REDISCACHEMANAGER_H
#define WEB_SERVER_REDISCACHEMANAGER_H


/* 设置异步redis驱动多长时间刷新一次缓存 默认300ms刷新一次 */

#ifndef REDIS_HANDLER_FLUSH_INTERVAL_IN_MICRISECONDS
#define REDIS_HANDLER_FLUSH_INTERVAL_IN_MICRISECONDS 300
#endif

#include <algorithm>
#include <vector>
#include <thread>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "DiskReader.h"
#include "Logger.h"
#include "cpp_redis"

class RedisCacheManager {
public:
    static void init(std::string redisOn,
                     std::string redisHost,
                     std::string redisPort,
                     std::string redisPass,
                     std::string redisDataBaseId,
                     std::string redisTTL);

    static void asyncResponse(std::shared_ptr<std::ostream> response, std::shared_ptr<std::string> fileName, std::function<void()> callback);
    static bool getCacheIsOpen();
    static void stop();
private:
    static bool redisIsOn;
    static std::string host;
    static std::string port;
    static std::string pass;
    static std::string dataBaseId;
    static int TTL;
    static cpp_redis::redis_client client;
    static boost::asio::io_service io_service;
    static std::function<void(cpp_redis::redis_client&)> disConnectCallback;
    static void respokseOK(std::ostream& response, const std::string& content);
    static void setTimer(boost::posix_time::microsec timeInterval);
};


#endif //WEB_SERVER_REDISCACHEMANAGER_H
