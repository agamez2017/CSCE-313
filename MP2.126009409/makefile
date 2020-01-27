# makefile

# uncomment the version of the compiler that you are using
#clang++ is for Mac OS 
#C++ = clang++ -std=c++11
# g++ is for most Linux
C++ = g++ -std=c++11

clean: 
	rm -f *.o dataserver simpleclient client 
	
all: dataserver simpleclient client 

reqchannel.o: reqchannel.hpp reqchannel.cpp
	$(C++) -c -g reqchannel.cpp

dataserver: dataserver.cpp reqchannel.o 
	$(C++) -o dataserver dataserver.cpp reqchannel.o -lpthread

simpleclient: simpleclient.cpp reqchannel.o
	$(C++) -o simpleclient simpleclient.cpp reqchannel.o

client: client.cpp reqchannel.o
	$(C++) -o client client.cpp reqchannel.o

mutex: mutex.cpp
	$(C++) -o mutex mutex.hpp mutex.cpp 

mutex_guard: mutex_guard.cpp
	$(C++) -o mutex_guard mutex_guard.hpp mutex_guard.cpp

semaphore: semaphore.cpp
	$(C++) -o semaphore semaphore.hpp semaphore.cpp