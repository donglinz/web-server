//
// Created by ubuntu on 17-2-28.
//

#ifndef WEB_SERVER_INITIALIZER_H
#define WEB_SERVER_INITIALIZER_H
#include <iostream>
#include <string>
#include <zconf.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <dirent.h>
#include <regex>
#include "tinyxml2.h"




namespace Configurations {
    enum Configuration{
        port,
        cacheSize,
        logPath,
        webRootPath,
        notFoundFile,
        publicKey,
        privateKey,
        httpServer,
        httpsServer,
        enableCache,
        ipBlackList,
        threadNum,
        useRedisCache,
        redisHost,
        redisPort,
        redisPass,
        redisDataBaseId,
        redisTTL,
        emptyKey
    };
}

class Initializer {
public:
    static void init();
    static std::map<Configurations::Configuration , std::string> config;
    static std::vector<std::string> ipBlackList;

private:

    static void initConfigMap();
    static void loadConfig(tinyxml2::XMLElement* root);
    static void serverConfig(tinyxml2::XMLElement* root);
    static void logConfig(tinyxml2::XMLElement* root);
    static void cacheConfig(tinyxml2::XMLElement* root);
    static bool checkConfigurations();
    static void showConfigurations(int fd);

    static void redisConfig(tinyxml2::XMLElement *pElement);
};


#endif //WEB_SERVER_INITIALIZER_H
