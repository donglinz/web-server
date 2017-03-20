//
// Created by ubuntu on 17-3-19.
//

#ifndef WEB_SERVER_IOSYSTEM_H
#define WEB_SERVER_IOSYSTEM_H
#include "CacheManager.h"
#include "RedisCacheManager.h"

class IOSystem {
public:
    enum CacheType { serverCache, redisCache };
    static void init(std::string notFoundFile,
                     std::string enableCache,
                     std::string cacheSize,
                     std::string useRedisCache,
                     std::string redisHost,
                     std::string redisPort,
                     std::string redisPass,
                     std::string redisDataBaseId,
                     std::string redisTTL);
    static std::string getReadBuffer(std::string & fileName, size_t & write_len);
    static void syncResponse(std::ostream & os, std::string & fileName, std::string & ipAddress);
    static void asyncResponse(std::ostream & os, std::string & fileName, std::string & ipAddress, std::function<void()> callback);
    static CacheType getCacheType();
private:
};



#endif //WEB_SERVER_IOSYSTEM_H
