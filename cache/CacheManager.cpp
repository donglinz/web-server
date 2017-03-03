//
// Created by ubuntu on 17-3-1.
//

#include "CacheManager.h"

#define ON "on"
#define OFF "off"

bool CacheManager::cacheIsOpen = false;
unsigned long CacheManager::maxMemorySize = 0;
unsigned long CacheManager::allocatedMemorySize = 0;

std::unordered_map<std::string, CacheManager::MemBlock *> CacheManager::nameToPtr;
std::map<CacheManager::MemBlock *, std::string> CacheManager::ptrToName;
CacheManager::LinkedList CacheManager::memlist = CacheManager::LinkedList();
std::mutex CacheManager::mutex;

void CacheManager::init(std::string enableCache, std::string cacheSize) {
    if(enableCache == OFF) {
        cacheIsOpen = false;
        return;
    }
    cacheIsOpen = true;
    allocatedMemorySize = 0;
    maxMemorySize = std::stoul(cacheSize) * 1024 * 1024;

}

/* 未解决BUG 若有一个文件比整个buffer大　就会返回404
 * 将buffer开的大一点可以避免这个问题*/
char *CacheManager::getReadBuffer(std::string & fileName) {
    /* threads concurrency */
    mutex.lock();

    /* cache命中 */
    if(nameToPtr.count(fileName)) {
        /* 修改内存块在LRU队列里的位置 */
        MemBlock *pos = nameToPtr[fileName];
        memlist.push_front(*pos);
        memlist.erase(pos);
        ptrToName.erase(pos);
        MemBlock *newPos = memlist.head.get();
        ptrToName[newPos] = fileName;
        nameToPtr[fileName] = newPos;
        return newPos->mem.get();
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
    if(allocatedMemorySize + length > maxMemorySize) {
        /* 内存不足,执行LRU置换算法 */
        do {
            if(memlist.size() == 0) {
                return nullptr;
            }
            MemBlock *ptr = memlist.tail.get();
            nameToPtr.erase(ptrToName[ptr]);
            ptrToName.erase(ptr);
            allocatedMemorySize -= strlen(ptr->mem.get()) + 1;
        } while(allocatedMemorySize + length > maxMemorySize);
        newMemBlock.mem = std::make_shared<char>(length);
    }
    newMemBlock.mem = std::make_shared<char>(length);
    memset(newMemBlock.mem.get(), 0, sizeof newMemBlock.mem.get());
    ifs.read(newMemBlock.mem.get(), length - 1);
    memlist.push_front(newMemBlock);
    ptrToName[memlist.head.get()] = fileName;
    nameToPtr[fileName] = memlist.head.get();
    ifs.close();
    return newMemBlock.mem.get();
}

void CacheManager::unlockMutex() {
    mutex.unlock();
}

bool CacheManager::getCacheIsOpen() {
    return cacheIsOpen;
}

