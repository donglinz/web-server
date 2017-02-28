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
#include "tinyxml2.h"
#include "dirent.h"



namespace Configurations {
    enum Configuration{
        port,
        cacheSize,
        logPath,
        errLogPath,
        webRootPath,
        notFoundPath,
        publicKey,
        privateKey,
        httpServer,
        httpsServer,
        enableCache,
        ipBlackList,
        emptyKey
    };
}

class Initializer {
public:
    static void init();

private:


    static std::map<Configurations::Configuration , std::string> config;
    static std::vector<std::string> ipBlackList;

    static void initConfigMap();
    static void loadConfig(tinyxml2::XMLElement* root);
    static void serverConfig(tinyxml2::XMLElement* root);
    static void logConfig(tinyxml2::XMLElement* root);
    static void cacheConfig(tinyxml2::XMLElement* root);
    static bool checkConfigurations();
    static void showConfigurations(int fd);
};


#endif //WEB_SERVER_INITIALIZER_H
