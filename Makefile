CC=g++ -Wall -std=c++14
obj=main.o ServerBase.o Server.o
lib=-lboost_system -lssl -lcrypto -lpthread
Server : $(obj)
	$(CC) -o Server $(obj) $(lib)
ServerBase.o : ServerBase.cpp
	$(CC) -c -o ServerBase.o ServerBase.cpp -lboost_system
Server.o : Server.cpp
	$(CC) -c -o Server.o Server.cpp 
main.o : main.cpp
	$(CC) -c -o main.o main.cpp

clean :
	rm -rf *.o Server