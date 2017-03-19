//
// Created by ubuntu on 17-3-19.
//

#ifndef WEB_SERVER_BASICCACHEMANAGER_H
#define WEB_SERVER_BASICCACHEMANAGER_H

#include <string>
#include <cstring>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <map>
#include <list>
#include <mutex>
#include <fstream>

class BasicCacheManager {
    virtual void init(std::string enableCache, std::string cacheSize) = 0;
    virtual std::string getReadBuffer(std::string & fileName, size_t & ret_length) = 0;
    virtual bool getCacheIsOpen() = 0;
};


#endif //WEB_SERVER_BASICCACHEMANAGER_H
