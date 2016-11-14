//============================================================================
// Name        : main.cpp
// Author      : Donglin Zhuang
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#pragma warning(disable:4996)  
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
#define DEB1
#ifdef DEB1
template<typename T>
class Vector {
public:
	Vector() : elements(0), first_free(0), end(0), size(0), capacity(0) {}

	Vector(const Vector& _rhs) {
		this->alloc = _rhs.alloc;
		this->elements = (this->alloc).allocate(_rhs.end - _rhs.elements);
		std::uninitialized_copy(_rhs.elements, _rhs.first_free, this->elements);
		this->first_free = this->elements + (_rhs.first_free - _rhs.elements);
		this->end = this->elements + (_rhs.end - _rhs.elements);
		this->size = _rhs.size;
		this->capacity = _rhs.capacity;
	}

	Vector& operator = (const Vector& _rhs) {
		this->alloc = _rhs.alloc;
		this->elements = (this->alloc).allocate(_rhs.end - _rhs.elements);
		std::uninitialized_copy(_rhs.elements, _rhs.first_free, this->elements);
		this->first_free = this->elements + (_rhs.first_free - _rhs.elements);
		this->end = this->elements + (_rhs.end - _rhs.elements);
		this->size = _rhs.size;
		this->capacity = _rhs.capacity;
		return *this;
	}

	


	~Vector() {
		while (first_free != elements) alloc.destroy(--first_free);
		alloc.deallocate(elements, end - elements);
	}

	void push_back(const T& t) {
		if (first_free == end) {
			reallocate();
		}
		alloc.construct(first_free, t);
		++first_free;
		++size;
	}

	void pop_back() {
		alloc.destroy(--first_free);
		--size;
	}

	T* Begin() {
		return elements;
	}

	T* End() {
		return first_free;
	}

	std::ptrdiff_t getSize() {
		return size;
	}

	std::ptrdiff_t getCapacity() {
		return capacity;
	}
	
	
private:
	std::allocator<T> alloc;
	void reallocate() {
		size = first_free - elements;
		std::ptrdiff_t newcapacity = 2 * std::max((int)size, 1);
		capacity = newcapacity;

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
	std::ptrdiff_t size;
	std::ptrdiff_t capacity;
};

int f(int &&a) {

	return 0;
}

int main() {
	//boost::asio::io_service io;
	//boost::asio::ip::tcp::socket socket(io);
	//unsigned short port = 8080;
	//boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
	//boost::asio::ip::tcp::acceptor acceptor(io, endpoint);
	//WebServerBase::Server<HTTP> a((unsigned short)(1), size_t(2));
	
	Vector<std::string> vec;
	std::string str = "1234567876543";
	for (int i = 0; i < 1000; i++) vec.push_back(str);
	while (true) {
		//for (int i = 0; i < 1000; i++) vec.push_back(str);
		//for (int i = 0; i < 1000; i++) vec.pop_back();
		Vector<std::string> v2 = vec;

	}

	system("pause");
	return 0;
}
#endif

#ifdef DEB2

int main() {
	std::vector<int> v;
	
	v.push_back(10);
	v.push_back(15);
	v.push_back(20);

	std::vector<int>::iterator i = v.begin();
	++i;

	std::vector<int>::iterator j = v.end();
	--j;

	std::cout << *j << '\n';

	v.insert(i, 25);

	std::cout << *j << '\n'; // Using an old iterator after an insert  
}
#endif


