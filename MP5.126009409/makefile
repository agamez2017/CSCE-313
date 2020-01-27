# makefile

# uncomment the version of the compiler that you are using
#clang++ is for Mac OS 
#C++ = clang++ -std=c++11
# g++ is for most Linux
C++ = g++ -std=c++11

clean: 
	rm -f *.o dataserver  client 
	
all: dataserver client 

netreqchannel.o: netreqchannel.hpp netreqchannel.cpp
	$(C++) -c -g netreqchannel.cpp

dataserver: dataserver.cpp netreqchannel.o 
	$(C++) -o dataserver dataserver.cpp netreqchannel.o -lpthread
	
client: client.cpp netreqchannel.o
	$(C++) -o client client.cpp netreqchannel.o -lpthread