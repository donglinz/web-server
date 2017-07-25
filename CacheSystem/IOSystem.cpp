//
// Created by ubuntu on 17-3-19.
//

#include "IOSystem.h"
IOSystem::CacheType IOSystem::cacheType;
//std::string IOSystem::getReadBuffer(std::string &fileName, size_t &write_len) {
//    return std::__cxx11::string();
//}

void IOSystem::init(std::string notFoundFile,
                    std::string enableCache,
                    std::string cacheSize,
                    std::string useRedisCache,
                    std::string redisHost,
                    std::string redisPort,
                    std::string redisPass,
                    std::string redisDataBaseId,
                    std::string redisTTL) {
    if(enableCache == OFF) {
        cacheType = noCache;
    } else if(useRedisCache == ON) {
        cacheType = redisCache;
    } else {
        cacheType = serverCache;
    }

    RedisCacheManager::init(useRedisCache, redisHost, redisPort, redisPass, redisDataBaseId, redisTTL);
    CacheManager::init(enableCache, cacheSize);
}

IOSystem::CacheType IOSystem::getCacheType() {
    return cacheType;
}

void IOSystem::syncResponse(std::ostream &os, const std::string &fileName, std::string &ipAddress) {
    Logger::LogNotification("Host from " + ipAddress + " Request file:" + fileName);
    if(cacheType == serverCache)
        CacheManager::getReadBuffer(fileName, os);
    else DiskReader::readFromDisk(fileName, os);
}

void IOSystem::asyncResponse(std::shared_ptr<std::ostream> os, std::shared_ptr<std::string> fileName, std::shared_ptr<std::string> ipAddress, std::function<void()> callback) {
    Logger::LogNotification("Host from " + *ipAddress + " Request file:" + *fileName);
    RedisCacheManager::asyncResponse(os, fileName, callback);
}
