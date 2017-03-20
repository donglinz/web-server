//
// Created by ubuntu on 17-3-19.
//

#ifndef WEB_SERVER_DISKREADER_H
#define WEB_SERVER_DISKREADER_H

#include <string>
#include <fstream>
#include "DiskReader.h"
class DiskReader {
public:
    static void readFromDisk(const std::string & fileName, std::ostream& response);
    static void init(std::string _notFoundFile);
    static void notFoundPage(std::ostream & ostream);
    static void cacheResponse(std::ostream & response, const char * cache, size_t len);
private:
    static std::string notFoundFile;
};


#endif //WEB_SERVER_DISKREADER_H
