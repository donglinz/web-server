//
// Created by ubuntu on 17-3-19.
//

#ifndef WEB_SERVER_IOSYSTEM_H
#define WEB_SERVER_IOSYSTEM_H
#include "CacheManager.h"
#include "RedisCacheManager.h"

class IOSystem {
public:
    static std::string getReadBuffer(std::string & fileName, size_t & write_len);
private:
    static std::unique_ptr<BasicCacheManager> cacher;
};


#endif //WEB_SERVER_IOSYSTEM_H
