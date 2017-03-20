//
// Created by ubuntu on 17-3-19.
//

#include "IOSystem.h"
std::unique_ptr<BasicCacheManager> IOSystem::cacheHandler;
std::string IOSystem::getReadBuffer(std::string &fileName, size_t &write_len) {
    return std::__cxx11::string();
}

void IOSystem::init(std::string enableCache,
                    std::string cacheSize,
                    std::string useRedisCache,
                    std::string redisHost,
                    std::string redisPort,
                    std::string redisPass,
                    std::string redisDataBaseId,
                    std::string redisTTL) {

}

IOSystem::CacheType IOSystem::getCacheType() {
    return redisCache;
}

void IOSystem::syncResponse(std::ostream &os, std::string &fileName, std::string &ipAddress) {

}

void IOSystem::asyncResponse(std::ostream &os, std::string &fileName, std::string &ipAddress,
                             std::function<void()> callback) {

}
