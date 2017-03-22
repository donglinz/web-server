//
// Created by ubuntu on 17-2-28.
//


#include "Initializer.h"

#define ON "on"
#define OFF "off"
#define DEFAULT_VALUE_PORT "12345"
#define DEFAULT_VALUE_ENABLE_CACHE OFF
#define DEFAULT_VALUE_CACHE_SIZE "10"
#define DEFAULT_VALUE_LOG_PATH "log/"
#define DEFAULT_VALUE_WEB_ROOT_PATH "web/"
#define DEFAULT_VALUE_NOT_FOUND_FILE ""
#define DEFAULT_VALUE_PUBLIC_KEY ""
#define DEFAULT_VALUE_PRIVATE_KEY ""
#define DEFAULT_VALUE_HTTP_SERVER OFF
#define DEFAULT_VALUE_HTTPS_SERVER OFF
#define DEFAULT_VALUE_IP_BLACK_LIST OFF
#define DEFAULT_VALUE_THREAD_NUM "1"
#define DEFAULT_VALUE_USE_REDIS_CACHE OFF
#define DEFAULT_VALUE_REDIS_HOST "127.0.0.1"
#define DEFAULT_VALUE_REDIS_PORT "6379"
#define DEFAULT_VALUE_REDIS_PASS ""
#define DEFAULT_VALUE_REDIS_DATABASE_ID "0"
#define DEFAULT_VALUE_REDIS_TTL "5"
#define DEFAULT_VALUE_REDIS_CONNECTION_POOL_SIZE "10"
#define STD_OUTPUT_STREAM 1
#define STD_ERROR_OUTPUT_STREAM 2
#define EMPTY_STRING ""

namespace Configurations {
    std::string confName[] = {
            "port",
            "cacheSize",
            "logPath",
            "webRootPath",
            "notFoundFile",
            "publicKey",
            "privateKey",
            "httpServer",
            "httpsServer",
            "enableCache",
            "ipBlackList",
            "threadNum",
            "useRedisCache",
            "redisHost",
            "redisPort",
            "redisPass",
            "redisDataBaseId",
            "redisTTL"
    };
    std::unordered_map<std::string, Configuration> confId = {
            {"port", port},
            {"cacheSize", cacheSize},
            {"logPath", logPath},
            {"webRootPath", webRootPath},
            {"notFoundFile", notFoundFile},
            {"publicKey", publicKey},
            {"privateKey", privateKey},
            {"httpServer", httpServer},
            {"httpsServer", httpsServer},
            {"enableCache", enableCache},
            {"ipBlackList", ipBlackList},
            {"threadNum", threadNum},
            {"useRedisCache", useRedisCache},
            {"redisHost", redisHost},
            {"redisPort", redisPort},
            {"redisPass", redisPass},
            {"redisDataBaseId", redisDataBaseId},
            {"redisTTL", redisTTL}
    };
};

std::map<Configurations::Configuration, std::string> Initializer::config = std::map<Configurations::Configuration, std::string>();
std::vector<std::string> Initializer::ipBlackList = std::vector<std::string>();

/* 从配置文件读取设置 */
void Initializer::init() {
    initConfigMap();
    tinyxml2::XMLDocument doc;
    if(doc.LoadFile("WebConfig.xml") != 0) {
        char *dir = getcwd(NULL, 0);
        std::cerr << "Cannot open config file WebConfig.xml";
        if(dir != nullptr) {
            std::cerr << " in directory " << dir;
        }
        std::cerr << std::endl;
        exit(1);
    }

    tinyxml2::XMLElement* root = doc.RootElement()->FirstChildElement();
    while(root != nullptr) {
        const char* content = root->Name();
        if(!strcmp(content, "server_config")) {
            serverConfig(root->FirstChildElement());
        } else if(!strcmp(content, "log_config")) {
            logConfig(root->FirstChildElement());
        } else if(!strcmp(content, "cache_config")) {
            cacheConfig(root->FirstChildElement());
        } else if(!strcmp(content, "redisConfig")) {
            redisConfig(root->FirstChildElement());
        }
        root = root->NextSiblingElement();
    }


    if(config[Configurations::ipBlackList] == ON) {
        root = doc.RootElement();
        root = root->FirstChildElement("ipBlackList")->FirstChildElement();
        while(root != nullptr) {
            ipBlackList.push_back(root->GetText());
            root = root->NextSiblingElement();
        }
    }

//    for(std::string s : ipBlackList) {
//        std::cout << s << std::endl;
//    }

    if(!checkConfigurations()) {
        showConfigurations(STD_ERROR_OUTPUT_STREAM);
        std::cerr << "Configurations error in file WebConfig.xml please check carefully." << std::endl;
        exit(1);
    }

    showConfigurations(STD_OUTPUT_STREAM);

    std::cout << "Starting server..." << std::endl;
}

/* 加载服务器设置 */
void Initializer::serverConfig(tinyxml2::XMLElement* root) {
    loadConfig(root);
}
/* 加载log设置 */
void Initializer::logConfig(tinyxml2::XMLElement* root) {
    loadConfig(root);
}
/* 加载cache设置 */
void Initializer::cacheConfig(tinyxml2::XMLElement* root) {
    loadConfig(root);
}
/* 加载redis设置 */
void Initializer::redisConfig(tinyxml2::XMLElement *root) {
    loadConfig(root);
}

/* 初始化默认设置 */
void Initializer::initConfigMap() {
    /* default port 12345 */
    config[Configurations::port] = DEFAULT_VALUE_PORT;
    /* default enable catch */
    config[Configurations::enableCache] = DEFAULT_VALUE_ENABLE_CACHE;
    /* default cache size 10mb */
    config[Configurations::cacheSize] = DEFAULT_VALUE_CACHE_SIZE;
    /* default log path ./log */
    config[Configurations::logPath] = DEFAULT_VALUE_LOG_PATH;
    /* default web root path */
    config[Configurations::webRootPath] = DEFAULT_VALUE_WEB_ROOT_PATH;
    /* default 404 page*/
    config[Configurations::notFoundFile] = DEFAULT_VALUE_NOT_FOUND_FILE;
    /* default public key */
    config[Configurations::publicKey] = DEFAULT_VALUE_PUBLIC_KEY;
    /* default private key */
    config[Configurations::privateKey] = DEFAULT_VALUE_PRIVATE_KEY;
    /* default http server */
    config[Configurations::httpServer] = DEFAULT_VALUE_HTTP_SERVER;
    /* default https server */
    config[Configurations::httpsServer] = DEFAULT_VALUE_HTTPS_SERVER;
    /* default ip black list is empty */
    config[Configurations::ipBlackList] = DEFAULT_VALUE_IP_BLACK_LIST;
    /* default thread num is 1 */
    config[Configurations::threadNum] = DEFAULT_VALUE_THREAD_NUM;
    /*  */
    config[Configurations::useRedisCache] = DEFAULT_VALUE_USE_REDIS_CACHE;
    /*  */
    config[Configurations::redisHost] = DEFAULT_VALUE_REDIS_HOST;
    /*  */
    config[Configurations::redisPort] = DEFAULT_VALUE_REDIS_PORT;
    /*  */
    config[Configurations::redisPass] = DEFAULT_VALUE_REDIS_PASS;
    /*  */
    config[Configurations::redisDataBaseId] = DEFAULT_VALUE_REDIS_DATABASE_ID;
    /*  */
    config[Configurations::redisTTL] = DEFAULT_VALUE_REDIS_TTL;
}

/* 检车设置是否合法 */
bool Initializer::checkConfigurations() {
    int port = std::stoi(config[Configurations::port]);
    if(port < 0 || port > UINT16_MAX) {
        std::cerr << "Config error!" << std::endl;
        return false;
    }

    /* 至少开启一个服务器 */
    if(config[Configurations::httpServer] == OFF &&
       config[Configurations::httpsServer] == OFF) {
        std::cerr << "Config error!" << std::endl;
        return false;
    }
    /* 不能一次开两个服务器 */
    if(config[Configurations::httpServer] == ON &&
       config[Configurations::httpsServer] == ON) {
        std::cerr << "Config error!" << std::endl;
        return false;
    }

    if(config[Configurations::httpsServer] == ON &&
       (config[Configurations::publicKey] == EMPTY_STRING ||
       config[Configurations::privateKey] == EMPTY_STRING)) {
        std::cerr << "Config error!" << std::endl;
        return false;
    }
    std::regex regex_ip("([0-9]{1,3}.){3}[0-9]{1,3}");
    if(config[Configurations::enableCache] == ON &&
            config[Configurations::useRedisCache] == ON) {
        if(!std::regex_match(config[Configurations::redisHost], regex_ip) ||
                config[Configurations::redisPort].empty() ||
                config[Configurations::redisDataBaseId].empty() ||
                config[Configurations::redisTTL].empty()) {
            std::cerr << "Config error!" << std::endl;
            return false;
        }
    }
    std::cout << "Config correct!" << std::endl;
    return true;
}

/* 显示当前服务器设置 fd==1是打印到标准输出,fd==2时打印到错误输出 */
void Initializer::showConfigurations(int fd) {
    std::ostream *out;
    if(fd == STD_OUTPUT_STREAM) out = &std::cout;
    else out = &std::cerr;
    std::ostream &outStream = *out;
    for(std::map<Configurations::Configuration, std::string>::iterator
            it = config.begin() ; it != config.end() ; ++it) {
        outStream << Configurations::confName[it->first] << " : "
                  << it->second << std::endl;
    }
}
/* 遍历XML数加载配置文件 */
void Initializer::loadConfig(tinyxml2::XMLElement* root) {
    while(root != nullptr) {
        std::string content = root->Name();
        Configurations::Configuration confid;
        if(Configurations::confId.count(content))
            confid = Configurations::confId[content];
        else confid = Configurations::emptyKey;

        /* 检查设置是否合法 */
        if(!config.count(confid)) {
            std::cerr << "Warning : Unsupported configuration.\n";
            std::cerr << "<" << content << ">";
            if(root->GetText() != nullptr)
                std::cerr << root->GetText();
            std::cerr << "</" << content << ">" << std::endl;
            root = root->NextSiblingElement();
            continue;
        }

        /* xml值为空 */
        if(root->GetText() == nullptr) {
            root = root->NextSiblingElement();
            continue;
        }
        std::string value = root->GetText();
        config[confid] = root->GetText();
        root = root->NextSiblingElement();
    }
}


