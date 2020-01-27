# makefile

# uncomment the version of the compiler that you are using
# clang++ is for Mac OS 
#C++ = clang++
# g++ is for most Linux
 C++ = g++ --std=c++11


all: memtest

clean:
	rm -f *.o

my_allocator.o : my_allocator.hpp my_allocator.cpp
	$(C++) -c -g my_allocator.cpp

free_list.o : free_list.hpp free_list.cpp
	$(C++) -c -g free_list.cpp

memtest: memtest.cpp my_allocator.o free_list.o
	$(C++) -o memtest memtest.cpp my_allocator.o free_list.o

