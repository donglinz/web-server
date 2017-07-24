//
// Created by ubuntu on 17-3-19.
//

#include "RedisCacheManager.h"

std::string RedisCacheManager::host;
std::string RedisCacheManager::port;
std::string RedisCacheManager::pass;
std::string RedisCacheManager::dataBaseId;
int RedisCacheManager::TTL;
bool RedisCacheManager::redisIsOn;
std::shared_ptr<cpp_redis::redis_client> RedisCacheManager::client;
boost::asio::io_service RedisCacheManager::io_service;
std::function<void(cpp_redis::redis_client&)> RedisCacheManager::disConnectCallback;


bool RedisCacheManager::getCacheIsOpen() {
    return redisIsOn;
}

void RedisCacheManager::init(std::string redisOn,
                             std::string redisHost,
                             std::string redisPort,
                             std::string redisPass,
                             std::string redisDataBaseId,
                             std::string redisTTL) {
    if(redisOn != "on") {
        redisIsOn = false;
        return;
    }
    client = std::make_shared<cpp_redis::redis_client>();
    redisIsOn = true;
    host = redisHost;
    port = redisPort;
    pass = redisPass;
    dataBaseId = redisDataBaseId;
    TTL = std::stoi(redisTTL);
    disConnectCallback = [](cpp_redis::redis_client & client)->void {
        Logger::LogError("Redis Client DisConnected from host " +
                                 host + ":" + port + " Reconnecting...");
        try {
            client.connect(host, std::stoul(port), disConnectCallback);
            if(!pass.empty()) {
                client.auth(pass);
                client.commit();
            }
        } catch( ... ) {
            Logger::LogError("Redis client error, cannot connect to host " +
            host + ":" + port);
        }
    };
    try {
        client->connect(host, std::stoul(port), disConnectCallback);
        if(!pass.empty()) {
            client->auth(pass);
            client->commit();
        }
    } catch( ... ) {
        Logger::LogError("Redis client error, cannot connect to host " +
                         host + ":" + port);
    }
    client->select(std::stoi(dataBaseId)).flushdb();
    client->commit();
    setTimer(boost::posix_time::millisec(REDIS_HANDLER_FLUSH_INTERVAL_IN_MICRISECONDS));

    std::thread([]()->void {
        io_service.run();
    }).detach();
}

void RedisCacheManager::setTimer(boost::posix_time::millisec timeInterval) {
    std::shared_ptr<boost::asio::deadline_timer> timerPtr =
            std::make_shared<boost::asio::deadline_timer>(io_service, timeInterval);

    timerPtr->async_wait([timeInterval, timerPtr](const boost::system::error_code & ec)->void {
        client->commit();
        setTimer(timeInterval);
    });
}

void RedisCacheManager::asyncResponse(std::shared_ptr<std::ostream> response,
                                      std::shared_ptr<std::string> fileName,
                                      std::function<void()> callback) {
    client->get(*fileName, [callback, response, fileName](cpp_redis::reply & reply)->void {
        if(reply.is_null() || reply.is_error() || !reply.is_string()) {
            std::string fileMsg = DiskReader::getStrFromDisk(*fileName);
            if(fileMsg.empty()) DiskReader::notFoundPage(*response);
            else {
                client->set(*fileName, fileMsg).expire(*fileName, TTL);
                responseOK(*response, fileMsg, fileName);
            }
        } else {
            responseOK(*response, reply.as_string(), fileName);
        }
        callback();
    });
}

void RedisCacheManager::responseOK(std::ostream& response, const std::string& content, std::shared_ptr<std::string> fileName) {
    response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length();
    std::string fn = *fileName;
    response << DiskReader::getContentType(*fileName);
    response << "\r\n\r\n" ;
    response.write(content.c_str(), content.length());
}

void RedisCacheManager::stop() {
    io_service.stop();
    if(client && client->is_connected())
        client->disconnect();
}

