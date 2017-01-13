CC:=g++ -Wall -std=c++14 -g
src:=main.cpp ServerBase.cpp ServerHttp.cpp
lib:=-lboost_system -lssl -lcrypto -lpthread
obj=$(src:.cpp=.o)
header=$(src:.cpp=.hpp)
deps=$(src:.cpp=.d)
Server : $(obj)
	$(CC) -o Server $(obj) $(lib)
	-rm -f *.d
$(obj) : %.o : %.cpp
	$(CC) -c -o $@ $<

sinclude $(deps)

$(deps) : %.d : %.cpp
	$(CC) -MM $< > $@;

.PHONY : clean
clean :
	-rm -f *.o Server
	-rm -f *.d