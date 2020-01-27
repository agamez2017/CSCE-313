/* 
    File: free_list.hpp

    Author: Adrian Gamez-Rodriguez
            Department of Computer Science and Engineering
            Texas A&M University
    Date  : 9/29/2019

    Modified:

*/

#ifndef _free_list_hpp_                   // include file only once
#define _free_list_hpp_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

typedef void * Addr;

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/
#include<cassert>
#include <cstdlib>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

class SegmentHeader {

public:

  static const unsigned COOKIE_VALUE = 0xBAAB00;
  unsigned int cookie; /* To check whether this is a genuine header! */
  size_t length;
  bool is_free;
  SegmentHeader* next;
  SegmentHeader* prev;
  //some data type l_or_r_field;
  //some data type inheritance;
  bool is_left;//0 for right and 1 for left child
  bool inh;//inheritance bit to build parent info
  

  SegmentHeader(size_t _length, bool _is_free = true, bool inh=false, bool is_left=false);

  ~SegmentHeader();
  /* We probably won't need the destructor. */

  void CheckValid();
  /* Check if the cookie is valid. */
};

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS  FreeList */
/*--------------------------------------------------------------------------*/

class FreeList {

 private:
	SegmentHeader* header;
	int size;
	int fib_num=0;

public:

  FreeList(int length); 
  /* This function initializes a new free-list. */

  ~FreeList(); 
  /* We probably don't need a destructor. */ 

  bool Remove(SegmentHeader * _segment); 
  /* Remove the given segment from the given free list. 
     Returns true if the function succeeds.
  */ 

  bool Add(SegmentHeader * _segment); 
  /* Add the segment to the given free list. */
  
  SegmentHeader* getHeader()
  {
	  return header;
  }
  
  int getsize()
  {
	return size;  
  }

  int get_Fib_num()
  {
    return fib_num;
  }
  
  
};

#endif 
