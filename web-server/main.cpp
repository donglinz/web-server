//============================================================================
// Name        : main.cpp
// Author      : Donglin Zhuang
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================



#include <iostream>
#include <cstdio>
#include <algorithm>
#include <regex>
#include <sstream>
#include <memory>
#include <unordered_map>
#include <functional>
#include <thread>
#include <vector>
#include <map>
#include <stack>
#include <allocators>
#include "Server.hpp"
#include "ServerBase.hpp"


template<typename T>
class Vector {
public:
	Vector() : elements(0), first_free(0), end(0) {}
	void push_back(const T& t) {
		if (first_free == end) {
			reallocate();
		}
		alloc.construct(first_free, t);
		++first_free;
	}

	
	
private:
	static std::allocator<T> alloc;
	void reallocate() {
		std::ptrdiff_t size = first_free - elements;
		std::ptrdiff_t newcapacity = 2 * std::max((int)size, 1);

		T *newelements = alloc.allocate(newcapacity);
		std::uninitialized_copy(elements, first_free, newelements);
		for (T* pt = first_free; pt != elements; ) {
			alloc.destroy(--pt);
		}

		if (elements) {
			alloc.deallocate(elements, end - elements);
		}

		elements = newelements;
		first_free = newelements + size;
		end = newelements + newcapacity;
	}
	T* elements;
	T* first_free;
	T* end;
};
int main() {
	//boost::asio::io_service io;
	//boost::asio::ip::tcp::socket socket(io);
	//unsigned short port = 8080;
	//boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
	//boost::asio::ip::tcp::acceptor acceptor(io, endpoint);
	//WebServerBase::Server<HTTP> a((unsigned short)(1), size_t(2));
	
	Vector<std::string> vec;
	std::string str = "1234567876543";
	for (int i = 0; i < 10000; i++) vec.push_back(str);
	std::cout << sizeof(std::allocator<std::string>) << std::endl;
	system("pause");
	return 0;
}