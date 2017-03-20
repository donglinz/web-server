//
// Created by ubuntu on 17-3-19.
//

#ifndef WEB_SERVER_REDISCACHEMANAGER_H
#define WEB_SERVER_REDISCACHEMANAGER_H

#include <algorithm>
#include <vector>
#include <thread>
#include "adapters/libevent.h"
#include "async.h"
#include "hiredis.h"
#include "BasicCacheManager.h"
#include "DiskReader.h"
#include "Logger.h"

class RedisCacheManager : public BasicCacheManager{
public:
    void init(std::string redisHost,
              std::string redisPort,
              std::string redisPass,
              std::string redisDataBaseId,
              std::string redisTTL);
    virtual std::string getReadBuffer(std::string & fileName, size_t & ret_length) override;
    virtual bool getCacheIsOpen() override;
private:
    std::string host;
    std::string port;
    std::string pass;
    std::string dataBaseId;
    std::string TTL;
    redisAsyncContext *asyncConnectionHandler;

    void connect();
    //bool isConnected(int id);
    std::string getErrorMsg(std::string msg);
    redisReply * redisCommandWapper(std::string command);
};


#endif //WEB_SERVER_REDISCACHEMANAGER_H
