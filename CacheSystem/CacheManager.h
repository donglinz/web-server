//
// Created by ubuntu on 17-3-1.
//

#ifndef WEB_SERVER_CACHEMANAGER_H
#define WEB_SERVER_CACHEMANAGER_H
#include "BasicCacheManager.h"
#include "DiskReader.h"
class CacheManager : public BasicCacheManager{
public:
    void init(std::string enableCache, std::string cacheSize);
    virtual std::string getReadBuffer(std::string & fileName, size_t & ret_length) override;
    virtual bool getCacheIsOpen() override;
    ~CacheManager();
private:
    bool cacheIsOpen;
    unsigned long maxMemorySize;
    unsigned long allocatedMemorySize;
    struct MemBlock {
        std::shared_ptr<char> mem;
        MemBlock *pre, *next;
        size_t length;
        MemBlock() {
            pre = next = nullptr;
            mem.reset();
        }
        MemBlock(size_t size) {
            mem = std::shared_ptr<char>(new char[size]);
            length = size - 1;
            memset(mem.get(), 0, size);
            pre = next = nullptr;
        }
        MemBlock(const MemBlock & rhs) {
            mem = rhs.mem;
            pre = rhs.pre;
            next = rhs.next;
        }
        MemBlock(MemBlock && rhs) = delete;
        MemBlock& operator = (const MemBlock & rhs) {
            mem = rhs.mem;
            pre = rhs.pre;
            next = rhs.next;
            return *this;
        }
        MemBlock& operator = (const MemBlock && rhs) = delete;
    };

    struct LinkedList {
        MemBlock *head, *tail;
        size_t length;
        LinkedList() {
            length = 0;
            head = tail = nullptr;
        }

        size_t size() {
            return length;
        }

        void pop_back() {
            erase(tail);
        }

        void erase(MemBlock *node, bool freeMemory = true) {
            --length;
            if(length == 0) {
                head = tail = nullptr;
                if(freeMemory) delete node;
                return ;
            }
            if(node ->pre != nullptr)
                node->pre->next = node->next;
            else head = head->next;
            if(node->next != nullptr)
                node->next->pre = node->pre;
            else tail = tail->pre;

            if(freeMemory) delete node;
        }

        void push_front(MemBlock *mem) {
            mem->pre = mem->next = nullptr;
            ++length;
            if(length == 1) {
                head = mem;
                tail = mem;
                mem->pre = nullptr;
                mem->next = nullptr;
                return ;
            }
            head->pre = mem;
            mem->next = head;
            head = mem;
        }
    };

    /* 内存页面置换采用LRU算法 memList为指向页面内存的指针 */

    std::unordered_map<std::string, MemBlock *> nameToPtr;
    std::map<MemBlock *, std::string> ptrToName;
    LinkedList memlist;
    std::mutex mutex;
};


#endif //WEB_SERVER_CACHEMANAGER_H
