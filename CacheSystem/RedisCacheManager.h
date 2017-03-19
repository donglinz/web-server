//
// Created by ubuntu on 17-3-19.
//

#ifndef WEB_SERVER_REDISCACHEMANAGER_H
#define WEB_SERVER_REDISCACHEMANAGER_H

#include "hiredis.h"
#include "BasicCacheManager.h"
class RedisCacheManager : public BasicCacheManager{
public:
    virtual void init(std::string enableCache, std::string cacheSize) override;
    virtual std::string getReadBuffer(std::string & fileName, size_t & ret_length) override;
    virtual bool getCacheIsOpen() override;

private:
};


#endif //WEB_SERVER_REDISCACHEMANAGER_H
