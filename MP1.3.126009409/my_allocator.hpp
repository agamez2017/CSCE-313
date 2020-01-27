/* 
    File: my_allocator.hpp

    Author: Adrian Gamez-Rodriguez
            Department of Computer Science and Engineering
            Texas A&M University
    Date  : 9/29/2019

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
#include<vector>

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
  char* orig_start; //points to the start of the memory block also used for the destructor
  char* endofMemory;//points to the end of memory we are going to manage
  size_t total_memory_length;
  size_t bbs;// basic block size
  size_t mem_size;//equal to the total length of the memory block we are going to manage.
  std::vector<FreeList> v_of_flists;// vector of the free list not inside the mem block allocated

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


  void Create_v_of_flist(size_t size);
  /*Creates the vector of freelists to use for the allocator*/
  
  void Merge(SegmentHeader* Segment1, SegmentHeader* Segment2);
  /* This function will merge two block if they are buddies and free into larger block*/
  
  void Split(SegmentHeader* Segment, int index);
  /*This function will split a block into two blocks.*/
  
  void PrintVector();
  /*This function prints out the all lists fib number and the number of free blocks at each list*/
  
  uint roundtoNearestFibsize(uint i);
  /*This function will round the input of malloc() to the nearest multiple of bbs and fibonacci block size;*/
  
  int findIndex(SegmentHeader* seg);
  /*This function maps out a size to an index of the vector of free lists*/
  
  SegmentHeader* getBuddy(SegmentHeader* Seg);
  /*This function will find the buddy of given segment*/

};

#endif 
