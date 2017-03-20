//
// Created by ubuntu on 17-3-1.
//

#include <Logger.h>
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


void CacheManager::getReadBuffer(std::string & fileName, std::ostream & response) {
    /* threads concurrency */
    std::unique_lock<std::mutex> lck(mutex);
    /* cache命中 */
    if(nameToPtr.count(fileName)) {
        /* 修改内存块在LRU队列里的位置 */
        MemBlock *pos = nameToPtr[fileName];
        memlist.erase(pos, false);
        memlist.push_front(pos);
        DiskReader::cacheResponse(response, pos->mem.get(), pos->length);
//        ret_length = pos->length;
//        return pos->mem.get();
        return;
    }

    std::ifstream ifs;
    ifs.open(fileName, std::ifstream::in | std::ifstream::binary);
    /* 404 Not Found */

    if(!ifs) {
        DiskReader::notFoundPage(response);
        return;
    }
    ifs.seekg(0, std::ios::end);

    /* 加1是因为末尾有\0 */
    size_t length = (size_t) ifs.tellg() + 1;
    ifs.seekg(0, std::ios::beg);
    if(length > maxMemorySize) {
        Logger::LogWarning("Cache size is too small!");
        DiskReader::readFromDisk(fileName, response);
        return;
    }

    MemBlock *newMemBlock;
    //std::cout << allocatedMemorySize << "\n" << maxMemorySize << std::endl;
    if(allocatedMemorySize + length > maxMemorySize) {
        /* 内存不足,执行LRU置换算法 */
        do {
            if(memlist.size() == 0) {
                Logger::LogWarning("Cache size is too small!");
                DiskReader::readFromDisk(fileName, response);
                return;
            }
            MemBlock *ptr = memlist.tail;
            allocatedMemorySize -= strlen(ptr->mem.get()) + 1;
            nameToPtr.erase(ptrToName[ptr]);
            ptrToName.erase(ptr);
            memlist.erase(ptr);
        } while(allocatedMemorySize + length > maxMemorySize);
    }
    try {
        newMemBlock = new MemBlock(length);
    } catch( ... ) {
        Logger::LogCritical("Fatal error, cannot allocating more memory!");
        exit(15);
    }

    allocatedMemorySize += length;
    ifs.read(newMemBlock->mem.get(), length - 1);

    memlist.push_front(newMemBlock);
    ptrToName[memlist.head] = fileName;
    nameToPtr[fileName] = memlist.head;
    ifs.close();

    DiskReader::cacheResponse(response, newMemBlock->mem.get(), length - 1);
//    ret_length = length - 1;
//    return newMemBlock->mem.get();
}
bool CacheManager::getCacheIsOpen() {
    return cacheIsOpen;
}



