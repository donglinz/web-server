//
// Created by ubuntu on 17-3-1.
//

#include "CacheManager.h"

#define ON "on"
#define OFF "off"

bool CacheManager::cacheIsOpen = false;
unsigned long CacheManager::maxMemorySize = 0;
unsigned long CacheManager::allocatedMemorySize = 0;

std::unordered_map<std::string, std::list<CacheManager::MemBlock>::iterator> CacheManager::nameToPtr;
std::map<std::list<CacheManager::MemBlock>::iterator, std::string> CacheManager::ptrToName;
std::list<CacheManager::MemBlock> CacheManager::memlist;
std::mutex CacheManager::mutex;


void CacheManager::init(std::string enableCache, std::string cacheSize) {
    if(enableCache == OFF) return;
    allocatedMemorySize = 0;
    maxMemorySize = std::stoul(cacheSize) * 1024 * 1024;

}

/* 未解决BUG 若有一个文件比整个buffer大　就会返回404 */
char *CacheManager::getReadBuffer(std::string & fileName) {
    /* threads concurrency */
    mutex.lock();

    /* cache命中 */
    if(nameToPtr.count(fileName)) {
        /* 修改内存块在LRU队列里的位置 */
        std::list<MemBlock>::iterator pos = nameToPtr[fileName];
        memlist.push_front(*pos);
        memlist.erase(pos);
        ptrToName.erase(pos);
//        std::list<MemBlock>::iterator newPos = memlist.begin();
//        ptrToName[newPos] = fileName;
//        nameToPtr[fileName] = newPos;

        return nameToPtr[fileName]->mem.get();
    }

    std::ifstream ifs;
    ifs.open(fileName, std::ifstream::in | std::ifstream::binary);
    /* 404 Not Found */
    if(!ifs) return nullptr;
    ifs.seekg(0, std::ios::end);

    /* 加1是因为末尾有\0 */
    size_t length = (size_t) ifs.tellg() + 1;
    ifs.seekg(0, std::ios::beg);
    if(length > maxMemorySize) return nullptr;

    MemBlock newMemBlock;
    if(allocatedMemorySize + length <= maxMemorySize)
        newMemBlock.mem = std::make_shared<char>(length);
    else {
        /* 内存不足,执行LRU置换算法 */
        do {


        } while(allocatedMemorySize + length > maxMemorySize);
        newMemBlock.mem = std::make_shared<char>(length);
    }

    ifs.close();
    return newMemBlock.mem.get();
}

void CacheManager::unlockMutex() {
    mutex.unlock();
}

bool CacheManager::getCacheIsOpen() {
    return cacheIsOpen;
}
