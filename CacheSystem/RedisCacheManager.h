//
// Created by ubuntu on 17-3-19.
//

#ifndef WEB_SERVER_REDISCACHEMANAGER_H
#define WEB_SERVER_REDISCACHEMANAGER_H


/* 设置异步redis驱动多长时间刷新一次缓存 默认300ms刷新一次 */
#define REDIS_HANDLER_FLUSH_INTERVAL_IN_MICRISECONDS 300

#include <algorithm>
#include <vector>
#include <thread>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "BasicCacheManager.h"
#include "DiskReader.h"
#include "Logger.h"
#include "cpp_redis"

class RedisCacheManager : public BasicCacheManager{
public:
    void init(std::string redisHost,
              std::string redisPort,
              std::string redisPass,
              std::string redisDataBaseId,
              std::string redisTTL);
    virtual std::string getReadBuffer(std::string & fileName, size_t & ret_length) override;
    void asyncResponse(std::ostream response, std::string fileName, std::function<void()> callback);
    virtual bool getCacheIsOpen() override;
private:
    std::string host;
    std::string port;
    std::string pass;
    std::string dataBaseId;
    std::string TTL;
    cpp_redis::redis_client client;
    boost::asio::io_service io_service;
    std::function<void(cpp_redis::redis_client&)> disConnectCallback;

    void setTimer(boost::posix_time::microsec timeInterval);

};


#endif //WEB_SERVER_REDISCACHEMANAGER_H
