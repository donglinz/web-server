//
// Created by ubuntu on 17-3-19.
//


#include "DiskReader.h"
std::string DiskReader::notFoundFile;
void DiskReader::readFromDisk(const std::string & fileName, std::ostream& response) {
    std::ifstream ifs;
    ifs.open(fileName, std::ifstream::in | std::ifstream::binary);
    if(ifs) {
        ifs.seekg(0, std::ios::end);
        size_t length = (size_t) ifs.tellg();

        ifs.seekg(0, std::ios::beg);

        // 文件内容拷贝到 response-stream 中，不应该用于大型文件
        response << "HTTP/1.1 200 OK\r\nContent-Length: " << length;
        if(fileName.length() >= 5 && fileName.substr(fileName.length() - 5) == ".html") response << "\r\nContent-Type:text/html";
        response << "\r\n\r\n" << ifs.rdbuf();

        ifs.close();
    } else {
        // 文件不存在时，返回无法打开文件
        notFoundPage(response);
    }
}

void DiskReader::notFoundPage(std::ostream &response, bool transpage) {
    if(notFoundFile.length() == 0 || !transpage) {
        std::string content="404 Not Found";
        response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
        return;
    }
    std::ifstream ifs;
    ifs.open(notFoundFile, std::ifstream::in | std::ifstream::binary);
    if(ifs) {
        ifs.seekg(0, std::ios::end);
        size_t length = (size_t) ifs.tellg();

        ifs.seekg(0, std::ios::beg);

        // 文件内容拷贝到 response-stream 中，不应该用于大型文件
        response << "HTTP/1.1 200 OK\r\nContent-Length: " << length;
        if(notFoundFile.length() >= 5 && notFoundFile.substr(notFoundFile.length() - 5) == ".html")
            response<< "\r\nContent-Type:text/html";
        response << "\r\n\r\n" << ifs.rdbuf();

        ifs.close();
    } else {
        // 文件不存在时，返回无法打开文件
        std::string content="404 Not Found";
        response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
    }
}

void DiskReader::init(std::string _notFoundFile) {
    notFoundFile = _notFoundFile;
}

void DiskReader::cacheResponse(std::ostream &response, const char *cache, size_t len, const std::string & fileName) {
    response << "HTTP/1.1 200 OK\r\nContent-Length: " << len;
    if(fileName.length() >= 5 && fileName.substr(fileName.length() - 5) == ".html")
        response<< "\r\nContent-Type:text/html";
    response << "\r\n\r\n";
    response.write(cache, len);
}

std::string DiskReader::getStrFromDisk(const std::string &fileName) {
    std::string fileMsg;
    std::ifstream ifs;
    ifs.open(fileName, std::ifstream::in | std::ifstream::binary);
    if(!ifs) return "";
    ifs.seekg(0, std::ios::end);

    /* 加1是因为末尾有\0 */
    size_t length = (size_t) ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    fileMsg.resize(length);
    ifs.read(const_cast<char *>(fileMsg.c_str()), length);
    return fileMsg;
}

std::string DiskReader::getContentType(const std::string &fileName) {
    if(fileName.length() >= 5 && fileName.substr(fileName.length() - 5) == ".html") return "\r\nContent-Type:text/html";
    return std::string();
}

