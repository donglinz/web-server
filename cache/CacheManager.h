//
// Created by ubuntu on 17-3-1.
//

#ifndef WEB_SERVER_CACHEMANAGER_H
#define WEB_SERVER_CACHEMANAGER_H
#include <string>
#include <cstring>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <map>
#include <list>
#include <mutex>
#include <fstream>
class CacheManager {
public:
    static void init(std::string enableCache, std::string cacheSize);
    static char* getReadBuffer(std::string & fileName);
    static void unlockMutex();
    static bool getCacheIsOpen();
private:
    static bool cacheIsOpen;
    static unsigned long maxMemorySize;
    static unsigned long allocatedMemorySize;

    struct MemBlock {
        std::shared_ptr<char> mem;
    };

    /* 内存页面置换采用LRU算法 memList为指向页面内存的指针 */
    static std::unordered_map<std::string, std::list<MemBlock>::iterator> nameToPtr;
    static std::map<std::list<MemBlock>::iterator, std::string> ptrToName;
    static std::list<MemBlock> memlist;
    static std::mutex mutex;



};


#endif //WEB_SERVER_CACHEMANAGER_H
