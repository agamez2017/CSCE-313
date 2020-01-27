/* 
    File: my_allocator.hpp

    Author: Adrian Gamez-Rodriguez
            Department of Computer Science and Engineering
            Texas A&M University
    Date  : 9/8/2019

    Modified: 
	
	Sources:PDF assignment, Source code given to us, TA in Lab

*/

#ifndef _my_allocator_hpp_                   // include file only once
#define _my_allocator_hpp_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/
#include"free_list.hpp"
#include <cstdlib>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

typedef void * Addr; 

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   MyAllocator */
/*--------------------------------------------------------------------------*/

class MyAllocator {

 private:

  // YOU MAY NEED TO ADD YOUR OWN DATA STRUCTURES HERE.
  char* start_addr; //points to the current free memory space
  char* end_addr;	//points to the end of the memory block allocated in constructor
  char* orig_start; //points to the start of the memory block also used for the destructor
  FreeList free_list; //the free list going to be used
  size_t bbs;
  size_t size;
  
 public:
  MyAllocator(size_t _basic_block_size, size_t _size);
  /* This function initializes the memory allocator and makes a portion of 
     ’_size’ bytes available. The allocator uses a ’_basic_block_size’ as 
     its minimal unit of allocation. 
     NOTE: In the old days one used 'unsigned int' for the size. The C
     Standards Committee has added the 'size_t' data type to ensure to 
     represent memory sizes. This makes the code more portable. Depending
     on the architecture, 'size_t' may or may not be the same 
     as 'unsigned int'. 
  */ 

  ~MyAllocator();
  /* This function returns any allocated memory to the operating system. 
  */ 


  Addr Malloc(size_t _length);
  /* Allocate _length number of bytes of free memory and returns the 
     address of the allocated portion. Returns nullptr when out of memory. 
  */ 

  bool Free(Addr _a); 
  /* Frees the section of physical memory previously allocated 
     using ’Malloc’. Returns true if everything ok. */ 
};

#endif 
